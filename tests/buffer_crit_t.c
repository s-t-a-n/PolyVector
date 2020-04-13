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
			ptr->free(ptr->v->peek(ptr));
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

Test(generic, pushfront_pushback)
{
		struct Buffer *ptr = vecnew(Buffer, 1, 3, free, strdup);
		cr_assert_not_null(ptr);

		void *str = strdup("String Middle");
		cr_assert(ptr->v->pushback(ptr, str) == 0);

		str = strdup("String Back");
		cr_assert(ptr->v->pushback(ptr, str) == 0);

		str = strdup("String Front");
		cr_assert(ptr->v->pushfront(ptr, str) == 0);

		cr_assert(ptr->v->size(ptr) == 3);

		cr_assert(strcmp(ptr->v->peek(ptr), "String Back") == 0);
		ptr->free(ptr->v->peek(ptr)); ptr->v->pop(ptr);
		cr_assert(ptr->v->size(ptr) == 2);

		cr_assert(strcmp(ptr->v->peek(ptr), "String Middle") == 0);
		ptr->free(ptr->v->peek(ptr)); ptr->v->pop(ptr);
		cr_assert(ptr->v->size(ptr) == 1);

		cr_assert(strcmp(ptr->v->peek(ptr), "String Front") == 0);
		ptr->free(ptr->v->peek(ptr)); ptr->v->pop(ptr);
		cr_assert(ptr->v->size(ptr) == 0);

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

struct t_struct {
	void *m1;
	size_t m1_len;
	void *m2;
	size_t m2_len;
};

static void *t_clone(void *_p)
{
	const struct t_struct *p = _p;

	struct t_struct *np = malloc(sizeof(struct t_struct));
	np->m1 = malloc(p->m1_len); memcpy(np->m1, p->m1, p->m1_len);
	np->m1_len = p->m1_len;
	np->m2 = malloc(p->m2_len); memcpy(np->m2, p->m2, p->m2_len);
	np->m2_len = p->m2_len;
	return(np);
}

static void t_free(void *_p)
{
	const struct t_struct *p = _p;

	free(p->m1);
	free(p->m2);
	free(_p);
}

Test(generic, free_clone)
{
	int cap = 1;
	int abscap = 3;

	struct Buffer *ptr = vecnew(Buffer, cap, abscap, t_free, t_clone);
	cr_assert_not_null(ptr);

	struct t_struct *p = malloc(sizeof(struct t_struct));
	p->m1_len = 1024;
	p->m1 = malloc(p->m1_len);
	p->m2_len = 1024;
	p->m2 = malloc(p->m2_len);

	sprintf(p->m1, "String m1");
	sprintf(p->m2, "String m2");

	sprintf(p->m1 + 512, "String m1 offset");
	sprintf(p->m2 + 512, "String m2 offset");
	
	cr_assert(ptr->v->push(ptr, p) == 0);
	cr_assert(ptr->v->size(ptr) == 1);

	struct Buffer *nptr = vecclone(ptr);

	cr_assert(strcmp(((struct t_struct *)(ptr->v->peek(ptr)))->m1, "String m1") == 0);
	cr_assert(strcmp(((struct t_struct *)(ptr->v->peek(ptr)))->m2, "String m2") == 0);
	cr_assert(strcmp(((struct t_struct *)(ptr->v->peek(ptr)))->m1 + 512, "String m1 offset") == 0);
	cr_assert(strcmp(((struct t_struct *)(ptr->v->peek(ptr)))->m2 + 512, "String m2 offset") == 0);

	cr_assert(strcmp(((struct t_struct *)(nptr->v->peek(nptr)))->m1, "String m1") == 0);
	cr_assert(strcmp(((struct t_struct *)(nptr->v->peek(nptr)))->m2, "String m2") == 0);
	cr_assert(strcmp(((struct t_struct *)(nptr->v->peek(nptr)))->m1 + 512, "String m1 offset") == 0);
	cr_assert(strcmp(((struct t_struct *)(nptr->v->peek(nptr)))->m2 + 512, "String m2 offset") == 0);

	t_flush_buffer(ptr);
	t_flush_buffer(nptr);
}
