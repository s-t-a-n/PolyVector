/*
 * =====================================================================================
 *
 *       Filename:  vector.c
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

#include "class.h"
#include "vector.h"

static void	*Vector_ctor(void *_self, va_list *ap)
{
	struct Vector		*self = _self;
	const unsigned char	*_mem = va_arg(*ap, const unsigned char *);
	const size_t		_size = va_arg(*ap, const size_t);
	const size_t		_cap= va_arg(*ap, const size_t);

	self->mem = malloc(_cap);
	self->size = _size;
	self->cap = _cap;
	if (self->mem)
		memcpy(self->mem, _mem, _size);
	return(self);
}

static void	*Vector_dtor(void *_self)
{
	struct Vector		*self = _self;

	free(self->mem);
	return (self);
}

/*
void	*Vector_clone(void *_self)
{
	struct Vector		*self = _self;
	
}
*/

const struct Class _Vector = {
	sizeof(struct Vector),
	Vector_ctor,
	Vector_dtor,
	NULL
};

const void *Vector = &_Vector;
