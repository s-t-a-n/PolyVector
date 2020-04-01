/*
 * =====================================================================================
 *
 *       Filename:  lifobuffer.h
 *
 *    Description:  static buffer with last-in first-out push/pop
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

#ifndef LIFOBUFFER_H
# define LIFOBUFFER_H

struct LiFoBuffer {
		const struct Vector	*v; /* must be first for dereference by class functions */
		void				(*free)(void *);
		void				*(*clone)(void *);
		void				**mem;
		size_t				cap;
		size_t				size;
		size_t				index;
};

extern const void			*LiFoBuffer;
#endif
