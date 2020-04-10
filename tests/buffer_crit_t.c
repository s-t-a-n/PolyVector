/*
 * =====================================================================================
 *
 *       Filename:  buffer_crit.c
 *
 *    Description:  Basic tests of buffer using Criterion
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
#include "types/buffer.h"

static void	t_flush_buffer(void *vec)
{
		struct Buffer *ptr = vec;
	
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
		void *ptr = vecnew(Buffer, 1, 1, free, strdup);
		cr_assert_not_null(ptr);
		vecdestroy(ptr);
}

Test(generic, clone)
{
		int tcount = 10;

		struct Buffer *ptr = vecnew(Buffer, tcount / 2, tcount * 2, free, strdup);
		cr_assert_not_null(ptr);


		for (int i = 0; i < tcount; i++)
		{
			char buf[16];
			snprintf(buf, 16, "String %i", i);

			void *str = strdup(buf);
			cr_assert(ptr->v->push(ptr, str) == 0);
		}

		struct Buffer *ptrc = vecclone(ptr);
		cr_assert_not_null(ptrc);

		for (int i = tcount - 1; i >= 0; i--)
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

Test(generic, cap_abccap)
{
		int cap = 1;
		int abscap = 3;
		struct Buffer *ptr = vecnew(Buffer, cap, abscap, free, strdup);
		cr_assert_not_null(ptr);

		void *str = strdup("String 0");
		cr_assert(ptr->v->push(ptr, str) == 0);

		str = strdup("String 1");
		cr_assert(ptr->v->push(ptr, str) == 0);

		str = strdup("String 2");
		cr_assert(ptr->v->push(ptr, str) == 0);

		str = strdup("String 3");
		cr_assert(ptr->v->push(ptr, str) > 0);
		free(str);

		cr_assert(ptr->size == 3);
		cr_assert(ptr->cap == 3);

		t_flush_buffer(ptr);
}

Test(generic, push_peek)
{
		struct Buffer *ptr = vecnew(Buffer, 1, 1, free, strdup);
		cr_assert_not_null(ptr);

		void *str = strdup("String");
		cr_assert(ptr->v->push(ptr, str) == 0);
		cr_assert(strcmp(ptr->v->peek(ptr), str) == 0);
		cr_assert(ptr->v->size(ptr) == 1);

		t_flush_buffer(ptr);
}

Test(generic, set_get)
{
		struct Buffer *ptr = vecnew(Buffer, 1, 1, free, strdup);
		cr_assert_not_null(ptr);

		void *str = strdup("String");
		cr_assert(ptr->v->push(ptr, str) == 0);
		cr_assert(strcmp(ptr->v->peek(ptr), str) == 0);

		str = strdup("gnirtS");
		cr_assert(ptr->v->set(ptr, 0, str) == 0);
		cr_assert(ptr->v->size(ptr) == 1);
		cr_assert(strcmp(ptr->v->get(ptr, 0), str) == 0);
		cr_assert(strcmp(ptr->v->peek(ptr), str) == 0);

		t_flush_buffer(ptr);
}

Test(generic, insert_remove)
{
		int cap = 1;
		int abscap = 3;

		struct Buffer *ptr = vecnew(Buffer, cap, abscap, free, strdup);
		cr_assert_not_null(ptr);
		void *str = strdup("String 0");
		cr_assert(ptr->v->push(ptr, str) == 0);

		str = strdup("String 2");
		cr_assert(ptr->v->push(ptr, str) == 0);

		cr_assert(ptr->v->size(ptr) == 2);
		cr_assert(strcmp(ptr->v->get(ptr, 0), "String 0") == 0);
		cr_assert(strcmp(ptr->v->get(ptr, 1), "String 2") == 0);
		cr_assert(ptr->v->insert(ptr, -1, "String f") > 0);
		cr_assert(ptr->v->insert(ptr, 3, "String f") > 0);

		str = strdup("String 1");
		cr_assert(ptr->v->insert(ptr, 1, str) == 0);
		cr_assert(ptr->v->size(ptr) == 3);
		cr_assert(strcmp(ptr->v->get(ptr, 0), "String 0") == 0);
		cr_assert(strcmp(ptr->v->get(ptr, 1), "String 1") == 0);
		cr_assert(strcmp(ptr->v->get(ptr, 2), "String 2") == 0);

		cr_assert(ptr->v->insert(ptr, 2, "String cap") > 0);

		ptr->v->remove(ptr, 1);
		cr_assert(ptr->v->size(ptr) == 2);
		cr_assert(strcmp(ptr->v->get(ptr, 0), "String 0") == 0);
		cr_assert(strcmp(ptr->v->get(ptr, 1), "String 2") == 0);

		t_flush_buffer(ptr);
}
