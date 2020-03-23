/*
 * =====================================================================================
 *
 *       Filename:  fifo_buffer.h
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

#ifndef FIFO_BUFFER_H
# define FIFO_BUFFER_H

struct FiFoBuffer {
		const struct Vector	*v; /* must be first for dereference by class functions */
		void				**mem;
		size_t				cap;
		size_t				size;
		size_t				front;
		size_t				back;
};

extern const void			*FiFoBuffer;
#endif
