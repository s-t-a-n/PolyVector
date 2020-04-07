/*
 * =====================================================================================
 *
 *       Filename:  buffer.c
 *
 *    Description:  dynamic buffer with arbitary access functions
 *
 *        Version:  1.0
 *        Created:  23-03-20 14:06:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stan Verschuuren (), sverschu@student.codam.nl
 *   Organization:  
 *
 * =====================================================================================
 */

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>

#include "vector.h"
#include "vector_errors.h"
#include "types/buffer.h"

static void		*ctor(void *_self, va_list *ap)
{
	struct Buffer	*self = _self;
	const size_t		_cap = va_arg(*ap, const size_t);
	const size_t		_abscap = va_arg(*ap, const size_t);
	const void			*_free = va_arg(*ap, const void *);
	const void			*_clone= va_arg(*ap, const void *);

	self->mem = calloc(_cap, sizeof(void *));
	if (!self->mem)
			return (NULL);
	self->size = 0;
	self->front = 0;
	self->back = -1;
	self->cap = _cap;
	self->abscap = _abscap;
	self->free = _free;
	self->clone = _clone;
	return (self);
}

static void		*dtor(void *_self)
{
	struct Buffer	*self = _self;

	while (self->size > 0)
	{
		self->free(self->v->peek(self));
		self->v->pop(self);
	}
	free(self->mem);
	return (self);
}

static void		*clone(void *_self)
{
	const struct Buffer	*self = _self;
	struct Buffer	*clone;

	clone = malloc(self->v->selfsize);
	if (clone)
	{
		clone->mem = calloc(self->cap, sizeof(void *));
		if (clone->mem)
		{
			clone->v = self->v;
			clone->cap = self->cap;
			clone->abscap = self->abscap;
			clone->size = self->size;
			clone->back = self->back;
			clone->free = self->free;
			clone->clone = self->clone;

			for (size_t i = 0; i < self->size; i++)
			{
				clone->mem[i] = self->clone(self->mem[i]);
			}
		}
		else
		{
			free(clone);
			clone = NULL;
		}
	}
	return (clone);
}

static int		grow(struct Buffer *self)
{
	size_t	newcap;
	void	*newmem;

	newcap = self->cap * 2;
	if (newcap > self->abscap && self->cap < self->abscap)
			newcap = self->abscap;
	if (newcap <= self->abscap)
	{
		newmem = realloc(self->mem, newcap * sizeof(void *));
		if (newmem)
		{
			self->cap = newcap;
			self->mem = newmem;
			return (0);
		}
		else
		{
			return (VEC_MAL);
		}
	}
	else
	{
		return (VEC_CAP);
	}
}

static int		pushback(void *_self, void *item)
{
	struct Buffer *self = _self;

	if (self->size >= self->cap)
	{
		int err = grow(self);
		if (err != 0)
			return (err);
	}

	(self->back)++;
	self->mem[self->back] = item;
	(self->size)++;
	return (0);
}

static int		pushfront(void *_self, void *item)
{
	struct Buffer *self = _self;

	if (self->size >= self->cap)
	{
		int err = grow(self);
		if (err != 0)
			return (err);
	}
	self->mem[self->front] = item;
	if (self->front == 0)
	{
		memmove(self->mem + 1, self->mem, self->size * sizeof(void *));
		(self->back)++;
	}
	else
		(self->front)--;
	(self->size)++;
	return (0);
}

static int		push(void *_self, void *item)
{
		return (pushback(_self, item));
}

static void		*peekback(void *_self)
{
	const struct	Buffer *self = _self;
	if (self->size > 0)
		return (self->mem[self->back]);
	else
		return (NULL);
}

static void		*peekfront(void *_self)
{
	const struct	Buffer *self = _self;
	if (self->size > 0)
		return (self->mem[self->front]);
	else
		return (NULL);
}

static void		*peek(void *_self)
{
	return(peekback(_self));
}

static void		popback(void *_self)
{
	struct	Buffer *self = _self;

	self->mem[self->back] = NULL; /* it is up to caller to free memory */
	if (self->size > 0)
	{
		if (self->back > 0)
			(self->back)--;
		(self->size)--;
	}
}

static void		popfront(void *_self)
{
	struct	Buffer *self = _self;

	self->mem[self->front] = NULL; /* it is up to caller to free memory */
	if (self->size > 0)
	{
		if (self->front < self->cap)
			(self->front)++;
		(self->size)--;
	}
}

static void		pop(void *_self)
{
	popback(_self);
}

static void		*get(void *_self, size_t _index)
{
	struct	Buffer *self = _self;

	if (_index >= 0 && _index < self->cap)
		return (self->mem[_index]);
	else
		return (NULL);
}

static int		set(void *_self, size_t _index, void *item)
{
	struct	Buffer *self = _self;
	
	if (_index < 0 || _index >= self->cap)
		return(VEC_RUB);
	self->free(self->mem[_index]);
	self->mem[_index] = item;
	return (0);
}

static int		insert(void *_self, size_t _index, void *item)
{
	struct Buffer *self = _self;

	if (_index < self->front || _index > self->back)
			return(VEC_RUB);
	if (self->size >= self->cap)
	{
		int err = grow(self);
		if (err != 0)
			return (err);
	}
	
	memmove(self->mem + _index + 1,
			self->mem + _index,
			(self->size - _index) * sizeof(void *));
	self->mem[_index] = item;
	(self->size)++;
	(self->back)++;
	return (0);
}

static void		remove(void *_self, size_t _index)
{
	struct	Buffer *self = _self;

	if (_index >= self->front && _index <= self->back)
	{
		self->free(self->mem[_index]);
		if (_index < self->back)
		{
			memmove(self->mem + _index,
					self->mem + _index + 1,
					(self->size - (_index + 1)) * sizeof(void *));
		}
		self->mem[self->back] = NULL;
		(self->back)--;
		(self->size)--;
	}
}

static size_t	size(void *_self)
{
	struct	Buffer *self = _self;

	return (self->size);
}

const struct Vector _Buffer = {
	sizeof(struct Buffer),
	ctor,
	dtor,
	clone,
	NULL,
	NULL,
	push,
	pushback,
	pushfront,
	peek,
	peekback,
	peekfront,
	pop,
	popback,
	popfront,
	get,
	set,
	insert,
	remove,
	size
};

const void *Buffer = &_Buffer;
