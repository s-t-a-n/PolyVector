/*
 * =====================================================================================
 *
 *       Filename:  vector.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23-03-20 13:36:07
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
		const void		*class; /* must be first for dereference by class functions */
		unsigned char	*mem;
		size_t			size;
		size_t			cap;
		void			(*pushback)(void *self, const unsigned char *mem);
};

extern const void		*Vector;
#endif
