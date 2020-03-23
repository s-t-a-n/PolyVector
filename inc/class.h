/*
 * =====================================================================================
 *
 *       Filename:  class.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23-03-20 12:46:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stan Verschuuren (), sverschu@student.codam.nl
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef CLASS_H
# define CLASS_H

struct Class {
	size_t	size;
	void	*(*ctor)(void *self, va_list *ap);
	void	*(*dtor)(void *self);
	void	*(*clone)(void *self);
};

void		*new(const void *_Class, ...);
void		destroy(void *self);
void		*clone(void *self);

#endif
