/*
 * =====================================================================================
 *
 *       Filename:  fifo_buffer.c
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
#include "fifo_buffer.h"

static void		*ctor(void *_self, va_list *ap)
{
	struct FiFoBuffer	*self = _self;
	const size_t			_cap= va_arg(*ap, const size_t);

	self->mem = malloc(_cap);
	self->size = 0;
	self->front = 0;
	self->back = -1;
	self->cap = _cap;
	return(self);
}

static void		*dtor(void *_self)
{
	struct FiFoBuffer	*self = _self;

	free(self->mem);
	return (self);
}

static void		*clone(void *self)
{
	return (self);
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

static int		pushback(void *_self, void *item)
{
	struct FiFoBuffer *self = _self;

	/*
	if (self->size + 1 >= self->cap)
	{
		if ((int err = grow(self)) != 0)
			return (err);
	}
	*/
	if (self->size < self->cap)
	{
		if (self->back < self->cap - 1)
		{
			(self->back)++;
			(self->size)++;
		}
		else
		{
			self->back = 0;
			(self->size)++;
		}
		self->mem[self->back] = item;
		return (0);
	}
	else
	{
		return(1);
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
	const struct	FiFoBuffer *self = _self;

	return(self->mem[self->front]);
}

static void		pop(void *_self)
{
	struct	FiFoBuffer *self = _self;

	self->mem[self->front] = NULL; /* it is up to caller to free  */
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

static void		*get(void *self, size_t index)
{
	return(NULL);
	(void)self;
	(void)index;
}

static int		set(void *self, size_t index, void *item)
{
	return(-254);
	(void)self;
	(void)index;
	(void)item;
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

const struct Vector _FiFoBuffer = {
	sizeof(struct FiFoBuffer),
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
	remove
};

const void *FiFoBuffer = &_FiFoBuffer;
