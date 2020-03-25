/*
 * =====================================================================================
 *
 *       Filename:  lafi_buffer.c
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
#include "lafi_buffer.h"

static void		*ctor(void *_self, va_list *ap)
{
	struct LaFiBuffer	*self = _self;
	const size_t			_cap= va_arg(*ap, const size_t);

	self->mem = malloc(_cap);
	if (!self->mem)
			return(NULL);
	self->index = -1;
	self->cap = _cap;
	return(self);
}

static void		*dtor(void *_self)
{
	struct LaFiBuffer	*self = _self;

	free(self->mem);
	return (self);
}

static void		*clone(void *self)
{
	return (self);
}

/*
static int		grow(struct LaFiBuffer *self)
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

static int		pushback(void *_self, void *item)
{
	struct LaFiBuffer *self = _self;

	if (self->index + 1 < self->cap)
	{
		(self->index)++;
		self->mem[self->index] = item;
		return (0);
	}
	else
	{
		return(VEC_FUL);
	}
}

static int		pushfront(void *self, void *item)
{
	return (-254);
	(void)self;
	(void)item;
}

static void		*peek(void *_self)
{
	const struct	LaFiBuffer *self = _self;

	return(self->mem[self->index]);
}

static void		pop(void *_self)
{
	struct	LaFiBuffer *self = _self;

	if (self->index >= 0)
		self->mem[self->index] = NULL; /* it is up to caller to free memory */
	if (self->index > 0)
		(self->index)--;
}

static void		*get(void *_self, size_t index)
{
	struct	LaFiBuffer *self = _self;

	if (index >= 0 && index < self->cap)
		return (self->mem[index]);
	else
		return (NULL);
}

static int		set(void *_self, size_t index, void *item)
{
	struct	LaFiBuffer *self = _self;
	
	if (index >= 0 && index < self->cap)
	{
		self->mem[index] = item;
		return (0);
	}
	else
	{
		return (VEC_RUB);
	}
}

static int		insert(void *self, size_t index, void *item)
{
	return(-254);
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
	struct  LaFiBuffer *self = _self;
	
	return (self->index + 1);
}
const struct Vector _LaFiBuffer = {
	sizeof(struct LaFiBuffer),
	ctor,
	dtor,
	clone,
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

const void *LaFiBuffer = &_LaFiBuffer;
