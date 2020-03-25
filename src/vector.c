/*
 * =====================================================================================
 *
 *       Filename:  vector.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23-03-20 13:00:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stan Verschuuren (), sverschu@student.codam.nl
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdarg.h>
#include <stdlib.h>

#include "vector.h"

void	*vecnew(const void *_Vector, ...)
{
	const struct Vector	*vector = _Vector;
	void				*obj;
	
	obj = malloc(vector->selfsize);
	if (obj)
	{
		/*  14: We  force  aconversion  ofpwhich  treats  the  beginning of  the  object as a pointer to astructClassand set the argumentclassas the value of this pointer.*/
		* (const struct Vector **)obj = vector;
		if(vector->ctor)
		{
			va_list ap;

			va_start(ap, _Vector);
			obj = vector->ctor(obj, &ap);
			va_end(ap);
		}
	}
	return (obj);
}

void	vecdestroy(void *self)
{
	const struct Vector	**obj = self;

	if (*obj && (*obj)->dtor)
	{
		self = (*obj)->dtor(self);
		/* 14: If an object does not want to be deleted,its destructor would return a null pointer */
		free(self);
	}
}

void	*vecclone(void *self)
{
	const struct Vector	**obj = self;

	if (*obj && (*obj)->clone)
		return((*obj)->clone(self));	
	else
		return (NULL);
}
