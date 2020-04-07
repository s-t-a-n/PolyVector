/*
 * =====================================================================================
 *
 *       Filename:  ringbuffer_mt_crit.c
 *
 *    Description:  Basic tests of threadsafe ringbuffer using Criterion
 *
 *        Version:  1.0
 *        Created:  23-03-20 14:34:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stan Verschuuren (), sverschu@student.codam.nl
 *   Organization:  
 *
 * =====================================================================================
 */

#include <criterion/criterion.h>
#include <criterion/logging.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "vector.h"
#include "types/ringbuffer_mt.h"

static void	t_flush_buffer(void *vec)
{
		struct RingBufferMT *ptr = vec;
	
		while (ptr->v->size(ptr) > 0)
		{
			free(ptr->v->peek(ptr));
			ptr->v->pop(ptr);
		}
}

Test(generic, init_destroy)
{
		void *ptr = vecnew(RingBufferMT, 1, 1, NULL, free, strdup);
		cr_assert_not_null(ptr);
		vecdestroy(ptr);
}

Test(generic, push_peek)
{
		struct RingBufferMT *ptr = vecnew(RingBufferMT, 1, free, strdup);
		cr_assert_not_null(ptr);
		void *str = strdup("String");
		int error = ptr->v->push(ptr, str);
		cr_assert(error == 0);
		cr_assert(strcmp(ptr->v->peek(ptr), str) == 0);
		cr_assert(ptr->v->size(ptr) == 1);
		t_flush_buffer(ptr);
		cr_assert(ptr->v->peek(ptr) == NULL);
		cr_assert(ptr->v->size(ptr) == 0);
		vecdestroy(ptr);
}

Test(generic, ring)
{
		struct RingBufferMT *ptr = vecnew(RingBufferMT, 2, free, strdup);
		cr_assert_not_null(ptr);

		void *str = strdup("String 0");
		int error = ptr->v->push(ptr, str);
		cr_assert(error == 0);

		str = strdup("String 1");
		error = ptr->v->push(ptr, str);
		cr_assert(error == 0);
		cr_assert(ptr->v->size(ptr) == 2);

		str = strdup("String 2");
		error = ptr->v->push(ptr, str);
		cr_assert(error == 0);
		cr_assert(ptr->v->size(ptr) == 2);

		cr_assert(strcmp(ptr->v->peek(ptr), "String 2") == 0);
		free(ptr->v->peek(ptr));
		ptr->v->pop(ptr);

		cr_assert(strcmp(ptr->v->peek(ptr), "String 1") == 0);
		free(ptr->v->peek(ptr));
		ptr->v->pop(ptr);

		cr_assert(ptr->v->peek(ptr) == NULL);
		cr_assert(ptr->v->size(ptr) == 0);
		vecdestroy(ptr);
}
