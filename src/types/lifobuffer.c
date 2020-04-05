/*
 * =====================================================================================
 *
 *       Filename:  lifobuffer.c
 *
 *    Description:  static buffer with last-in first-out push/pop
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
#include "types/lifobuffer.h"

static void		*ctor(void *_self, va_list *ap)
{
	struct LiFoBuffer	*self = _self;
	const size_t		_cap = va_arg(*ap, const size_t);
	const void			*_free = va_arg(*ap, const void *);
	const void			*_clone = va_arg(*ap, const void *);
	self->mem = malloc(_cap * sizeof(void *));
	if (!self->mem)
			return (NULL);
	self->size = 0;
	self->index = -1;
	self->cap = _cap;
	self->free = _free;
	self->clone = _clone;
	return (self);
}

static void		*dtor(void *_self)
{
	struct LiFoBuffer	*self = _self;

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
	const struct LiFoBuffer *self = _self;
	struct LiFoBuffer		*clone;

	clone = malloc(self->v->selfsize);
	if (clone)
	{
		clone->mem = malloc(self->cap * sizeof(void *));
		if (clone->mem)
		{
			clone->v = self->v;
			clone->cap = self->cap;
			clone->size = self->size;
			clone->index = self->index;
			clone->free = self->free;
			clone->clone = self->clone;

			for (size_t i = 0; i < self->size; i++)
				clone->mem[i] = self->clone(self->mem[i]);
		}
		else
		{
			free(clone);
			clone = NULL;
		}
	}
	return (clone);
}

static int		push(void *_self, void *item)
{
	struct LiFoBuffer *self = _self;

	if (self->size < self->cap)
	{
		(self->index)++;
		self->mem[self->index] = item;
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

static void		*peekback(void *_self)
{
	const struct	LiFoBuffer *self = _self;

	if (self->size > 0)
		return (self->mem[self->index]);
	else
		return (NULL);
}

static void		*peekfront(void *_self)
{
	return (NULL);
	(void)_self;
}

static void		*peek(void *_self)
{
	return (peekback(_self));
}

static void		popback(void *_self)
{
	struct	LiFoBuffer *self = _self;

	if (self->size > 0)
	{
		self->mem[self->index] = NULL; /* it is up to caller to free memory */
		(self->index)--;
		(self->size)--;
	}
}

static void		popfront(void *_self)
{
	(void)_self;
}
static void		pop(void *_self)
{
	popback(_self);
}

static void		*get(void *_self, size_t index)
{
	return (NULL);
	(void)_self;
	(void)index;
}

static int		set(void *_self, size_t index, void *item)
{
	return (VEC_STB);
	(void)_self;
	(void)index;
	(void)item;
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

static size_t   size(void *_self)
{
	struct  LiFoBuffer *self = _self;
	
	return (self->size);
}
const struct Vector _LiFoBuffer = {
	sizeof(struct LiFoBuffer),
	ctor,
	dtor,
	clone,
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

const void *LiFoBuffer = &_LiFoBuffer;
