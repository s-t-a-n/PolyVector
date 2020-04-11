/*
 * =====================================================================================
 *
 *       Filename:  ringbuffer_crit.c
 *
 *    Description:  Basic tests of ringbuffer using Criterion
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
#include "types/ringbuffer.h"

static void	t_flush_buffer(void *vec)
{
		struct RingBuffer *ptr = vec;
	
		while (ptr->v->size(ptr) > 0)
		{
			free(ptr->v->peek(ptr));
			ptr->v->pop(ptr);
		}

		cr_assert(ptr->v->peek(ptr) == NULL);
		cr_assert(ptr->v->size(ptr) == 0);
		vecdestroy(ptr);
}

Test(generic, init_destroy)
{
		void *ptr = vecnew(RingBuffer, 1, 1, NULL, free, strdup);
		cr_assert_not_null(ptr);
		vecdestroy(ptr);
}

Test(generic, clone)
{
		int tcount = 10;

		struct RingBuffer *ptr = vecnew(RingBuffer, tcount, free, strdup);
		cr_assert_not_null(ptr);

		for (int i = 0; i < tcount; i++)
		{
			char buf[16];
			snprintf(buf, 16, "String %i", i);

			void *str = strdup(buf);
			cr_assert(ptr->v->push(ptr, str) == 0);
		}

		/* overwrite test */
		cr_assert(ptr->v->push(ptr, strdup("Overwrite")) == 0);

		struct RingBuffer *ptrc = vecclone(ptr);
		cr_assert_not_null(ptrc);

		void *retc = ptrc->v->peek(ptrc);
		cr_assert_not_null(retc);
		cr_assert(strcmp(retc, "Overwrite") == 0);
		ptrc->v->pop(ptrc);

		void *ret = ptr->v->peek(ptr);
		cr_assert_not_null(ret);
		cr_assert(strcmp(ret, "Overwrite") == 0);
		ptrc->v->pop(ptr);

		free(ret);
		free(retc);

		for (int i = 1; i < tcount; i++)
		{
			char buf[16];
			snprintf(buf, 16, "String %i", i);

			void *retc = ptrc->v->peek(ptrc);
			cr_assert_not_null(retc);
			cr_assert(strcmp(retc, buf) == 0);
			ptrc->v->pop(ptrc);

			void *ret = ptr->v->peek(ptr);
			cr_assert_not_null(ret);
			cr_assert(strcmp(ret, buf) == 0);
			ptrc->v->pop(ptr);

			free(ret);
			free(retc);
		}

		t_flush_buffer(ptr);
		t_flush_buffer(ptrc);
}

Test(generic, push_peek)
{
		struct RingBuffer *ptr = vecnew(RingBuffer, 1, free, strdup);
		cr_assert_not_null(ptr);
		void *str = strdup("String");
		cr_assert(ptr->v->push(ptr, str) == 0);
		cr_assert(strcmp(ptr->v->peek(ptr), str) == 0);
		cr_assert(ptr->v->size(ptr) == 1);
		t_flush_buffer(ptr);
}

Test(generic, ring)
{
		struct RingBuffer *ptr = vecnew(RingBuffer, 2, free, strdup);
		cr_assert_not_null(ptr);

		void *str = strdup("String 0");
		cr_assert(ptr->v->push(ptr, str) == 0);

		str = strdup("String 1");
		cr_assert(ptr->v->push(ptr, str) == 0);
		cr_assert(ptr->v->size(ptr) == 2);

		str = strdup("String 2");
		cr_assert(ptr->v->push(ptr, str) == 0);
		cr_assert(ptr->v->size(ptr) == 2);

		cr_assert(strcmp(ptr->v->peek(ptr), "String 2") == 0);
		free(ptr->v->peek(ptr));
		ptr->v->pop(ptr);

		cr_assert(strcmp(ptr->v->peek(ptr), "String 1") == 0);

		t_flush_buffer(ptr);
}
