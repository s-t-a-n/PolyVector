/*
 * =====================================================================================
 *
 *       Filename:  ringbuffer.h
 *
 *    Description:  static ringbuffer with first-in first-out push/pop
 *    				(frees and overwrites when buffer is full)
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

#ifndef RINGBUFFER_H
# define RINGBUFFER_H

struct RingBuffer {
		const struct Vector	*v; /* must be first for dereference by class functions */
		void				(*free)(void *);
		void				*(*clone)(void *);
		void				**mem;
		size_t				cap;
		size_t				size;
		size_t				front;
		size_t				back;
};

extern const void			*RingBuffer;
#endif
