/*
 * =====================================================================================
 *
 *       Filename:  vector.h
 *
 *    Description: Vector class prototypes
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

#ifndef VECTOR_H
# define VECTOR_H

struct Vector {
	size_t	selfsize;
	void	*(*ctor)(void *, va_list *);
	void	*(*dtor)(void *);
	void	*(*clone)(void *);
	void	*(*safe_pop)(void *);
	int		(*safe_push)(void *, void *);
	int		(*push)(void *, void *);
	int		(*pushback)(void *, void *);
	int		(*pushfront)(void *, void *);
	void	*(*peek)(void *);
	void	*(*peekback)(void *);
	void	*(*peekfront)(void *);
	void	(*pop)(void *);
	void	(*popback)(void *);
	void	(*popfront)(void *);
	void	*(*get)(void *, size_t );
	int		(*set)(void *, size_t , void *);
	int		(*insert)(void *, size_t , void *);
	void	(*remove)(void *, size_t);
	size_t	(*size)(void *);
};

void		*vecnew(const void *_Vector, ...);
void		vecdestroy(void *self);
void		*vecclone(void *self);

#endif
