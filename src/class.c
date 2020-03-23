/*
 * =====================================================================================
 *
 *       Filename:  class.c
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

#include "class.h"

void	*new(const void *_Class, ...)
{
	const struct Class	*class = _Class;
	void				*obj;
	
	obj = malloc(class->size);
	if (obj)
	{
		/*  14: We  force  aconversion  ofpwhich  treats  the  beginning of  the  object as a pointer to astructClassand set the argumentclassas the value of this pointer.*/
		* (const struct Class **)obj = class;
		if(class->ctor)
		{
			va_list ap;

			va_start(ap, _Class);
			obj = class->ctor(obj, &ap);
			va_end(ap);
		}
	}
	return (obj);
}

void	destroy(void *self)
{
	const struct Class	**obj = self;

	if (*obj && (*obj)->dtor)
	{
		self = (*obj)->dtor(self);
		/* 14: If an object does not want to be deleted,its destructor would return a null pointer */
		free(self);
	}
}

void	*clone(void *self)
{
	const struct Class	**obj = self;

	if (*obj && (*obj)->clone)
		return((*obj)->clone(self));	
	else
		return (NULL);
}
