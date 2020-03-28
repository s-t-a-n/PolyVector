/*
 * =====================================================================================
 *
 *       Filename:  fifobuffer.c
 *
 *    Description:  
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
#include "fifobuffer.h"

static void		*ctor(void *_self, va_list *ap)
{
	struct FiFoBuffer	*self = _self;
	const size_t		_cap = va_arg(*ap, const size_t);
	const void			*_free = va_arg(*ap, const void *);
	const void			*_clone= va_arg(*ap, const void *);

	self->mem = malloc(_cap * sizeof(void *));
	if (!self->mem)
			return (NULL);
	self->size = 0;
	self->front = 0;
	self->back = -1;
	self->cap = _cap;
	self->free = _free;
	self->clone = _clone;
	return (self);
}

static void		*dtor(void *_self)
{
	struct FiFoBuffer	*self = _self;

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
	const struct FiFoBuffer	*self = _self;
	struct FiFoBuffer	*clone;

	clone = malloc(self->v->selfsize);
	if (clone)
	{
		clone->mem = malloc(self->cap * sizeof(void *));
		if (clone->mem)
		{
			clone->v = self->v;
			clone->cap = self->cap;
			clone->size = self->size;
			clone->front = self->front;
			clone->back = self->cap;
			clone->free = self->free;
			clone->clone = self->clone;

			for (size_t i = 0; i < self->size; i++)
			{
				if (self->front + i >= self->cap)
				{
					const int j = self->front + i - self->cap;
					clone->mem[j] = self->clone(self->mem[j]);
				}
				else
				{
					const int k = self->front + i;
					clone->mem[k] = self->clone(self->mem[k]);
				}
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

/*
static int		grow(struct FiFoBuffer *self)
{
	size_t newcap;
	void *new;

	newcap = self->cap * 2;
	if (newcap <= self->abscap)
	{
		new = realloc(self->mem, newcap);
		if (new)
		{
			memcpy(new, self->mem, self->size);
			self->cap = newcap;
			return (0);
		}
		else
		{
			return (1);
		}
	}
	else
	{
		return (2);
	}
}
*/

/*
if (self->size + 1 >= self->cap)
{
	if ((int err = grow(self)) != 0)
		return (err);
}
*/

static int		push(void *_self, void *item)
{
	struct FiFoBuffer *self = _self;

	if (self->size < self->cap)
	{
		if (self->back < self->cap - 1)
			(self->back)++;
		else
			self->back = 0;
		self->mem[self->back] = item;
		(self->size)++;
		return (0);
	}
	else
	{
		return (VEC_FUL);
	}
}
static int		pushback(void *_self, void *item)
{
	return (push(_self, item));
}

static int		pushfront(void *self, void *item)
{
	return (VEC_STB);
	(void)self;
	(void)item;
}

static void		*peek(void *_self)
{
	const struct	FiFoBuffer *self = _self;
	if (self->size > 0)
		return (self->mem[self->front]);
	else
		return (NULL);
}

static void		pop(void *_self)
{
	struct	FiFoBuffer *self = _self;

	self->mem[self->front] = NULL; /* it is up to caller to free memory */
	if (self->size > 0)
	{
		if (self->front < self->cap - 1)
		{
			(self->front)++;
		}
		else
		{
			self->front = 0;
		}
		(self->size)--;
	}
}

static void		*get(void *_self, size_t index)
{
	return (NULL);
	(void)_self;
	(void)index;

	/*
	struct	FiFoBuffer *self = _self;

	if (index >= 0 && index < self->cap)
		return (self->mem[index]);
	else
		return (NULL);
	*/
}

static int		set(void *_self, size_t index, void *item)
{
	return (VEC_STB);
	(void)_self;
	(void)index;
	(void)item;

	/*
	struct	FiFoBuffer *self = _self;
	
	if (index >= 0 && index < self->cap)
	{
		self->free(self->mem[index]);
		self->mem[index] = item;
		return (0);
	}
	else
	{
		return (VEC_RUB);
	}
	*/
}

static int		insert(void *self, size_t index, void *item)
{
	return (VEC_STB);
	(void)self;
	(void)index;
	(void)item;
}

static void		remove(void *self, size_t index)
{
	(void)self;
	(void)index;
}

static size_t	size(void *_self)
{
	struct	FiFoBuffer *self = _self;

	return (self->size);
}

const struct Vector _FiFoBuffer = {
	sizeof(struct FiFoBuffer),
	ctor,
	dtor,
	clone,
	push,
	pushback,
	pushfront,
	peek,
	pop,
	get,
	set,
	insert,
	remove,
	size
};

const void *FiFoBuffer = &_FiFoBuffer;
