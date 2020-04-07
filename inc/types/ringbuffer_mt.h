/*
 * =====================================================================================
 *
 *       Filename:  ringbuffer_mt.h
 *
 *    Description:  static ringbuffer with first-in first-out push/pop
 *    				(frees and overwrites when buffer is full)
 *    				multithreading safe
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

#ifndef RINGBUFFER_MT_H
# define RINGBUFFER_MT_H

struct RingBufferMT {
	const struct Vector	*v; /* must be first for dereference by class functions */
	void				(*free)(void *);
	void				*(*clone)(void *);
	void				**mem;
	size_t				cap;
	size_t				size;
	size_t				front;
	size_t				back;
	pthread_mutex_t		lock;
	pthread_cond_t		signal;
};

extern const void			*RingBufferMT;
#endif
