/*
 * =====================================================================================
 *
 *       Filename:  lafi_buffer.h
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

#ifndef LAFI_BUFFER_H
# define LAFI_BUFFER_H

struct LaFiBuffer {
		const struct Vector	*v; /* must be first for dereference by class functions */
		void				**mem;
		size_t				cap;
		size_t				index;
};

extern const void			*LaFiBuffer;
#endif
