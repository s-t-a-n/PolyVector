/*
 * =====================================================================================
 *
 *       Filename:  ringbuffer_mt.c
 *
 *    Description:  static ringbuffer with first-in first-out push/pop
 *    				(frees and overwrites when buffer is full)
 *    				multithreading safe
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

#include <pthread.h>
#include <signal.h> 

#include "vector.h"
#include "vector_errors.h"
#include "types/ringbuffer_mt.h"

static void		*ctor(void *_self, va_list *ap)
{
	struct RingBufferMT	*self = _self;
	const size_t		_cap = va_arg(*ap, const size_t);
	const void			*_free = va_arg(*ap, const void *);
	const void			*_clone= va_arg(*ap, const void *);

	if (pthread_mutex_init(&self->lock, NULL) != 0 ||
		pthread_cond_init(&self->signal, NULL) != 0)
			return(NULL);
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
	struct RingBufferMT	*self = _self;

	pthread_mutex_destroy(&self->lock);
	pthread_cond_destroy(&self->signal);
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
	const struct RingBufferMT	*self = _self;
	struct RingBufferMT	*clone;

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
			clone->back = self->back;
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

static int		pushback(void *_self, void *item)
{
	struct RingBufferMT *self = _self;

	if (self->back < self->cap - 1)
		(self->back)++;
	else
		self->back = 0;
	if (self->size >= self->cap)
		self->free(self->mem[self->back]);
	else
		(self->size)++;
	self->mem[self->back] = item;
	return (0);
}

static int		pushfront(void *self, void *item)
{
	return (VEC_STB);
	(void)self;
	(void)item;
}

static int		push(void *_self, void *item)
{
	return (pushback(_self, item));
}

static void		*peekback(void *_self)
{
	return (NULL);
	(void)_self;
}

static void		*peekfront(void *_self)
{
	const struct	RingBufferMT *self = _self;
	if (self->size > 0)
		return (self->mem[self->front]);
	else
		return (NULL);
}

static void		*peek(void *_self)
{
	return (peekfront(_self));
}

static void		popback(void *_self)
{
	(void)_self;	
}

static void		popfront(void *_self)
{
	struct	RingBufferMT *self = _self;

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

static void		pop(void *_self)
{
	popfront(_self);
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

static size_t	size(void *_self)
{
	struct	RingBufferMT *self = _self;

	return (self->size);
}

static void		*safe_get(void *_self)
{
	struct RingBufferMT *self = _self;
	void *element;

	if (pthread_mutex_lock(&self->lock) == 0)
	{
		while (self->size == 0)
			pthread_cond_wait(&self->signal, &self->lock);
		element = peek(self);
		if (element)
			pop(self);
		pthread_mutex_unlock(&self->lock);
		return (element);
	}
	else
		return(NULL);
}

static int		safe_add(void *_self, void *element)
{
	struct RingBufferMT *self = _self;
	int					error;

	error = pthread_mutex_lock(&self->lock);
	if (error == 0)
	{
			error += push(self, element);
			error += pthread_mutex_unlock(&self->lock);
			error += pthread_cond_signal(&self->signal);
	}
	return(abs(error));
}

const struct Vector _RingBufferMT = {
	sizeof(struct RingBufferMT),
	ctor,
	dtor,
	clone,
	safe_get,
	safe_add,
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

const void *RingBufferMT = &_RingBufferMT;
