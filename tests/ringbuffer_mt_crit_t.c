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

/* required for nonblocking pthread_join */
#define _GNU_SOURCE

#include <criterion/criterion.h>
#include <criterion/logging.h>

#include <pthread.h>
#include <signal.h>

/* required for sleep() */
#include <unistd.h>

#include <errno.h>

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
		cr_assert(ptr->v->peek(ptr) == NULL);
		cr_assert(ptr->v->size(ptr) == 0);
		vecdestroy(ptr);
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
		cr_assert(ptr->v->push(ptr, str) == 0);
		cr_assert(strcmp(ptr->v->peek(ptr), str) == 0);
		cr_assert(ptr->v->size(ptr) == 1);

		t_flush_buffer(ptr);
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

		t_flush_buffer(ptr);
}

static void *t_main(void *_ptr)
{
	struct RingBufferMT *ptr = _ptr;

	void *item = ptr->v->safe_get(ptr);
	cr_assert(item != NULL);
	return(item);
}

Test(threading, safeset_safeget)
{
		int tcount = 5;

		struct RingBufferMT *ptr = vecnew(RingBufferMT, tcount, free, strdup);
		cr_assert_not_null(ptr);

		pthread_t threads[tcount];
		for (int i = 0; i < tcount; i++)
		{
			 cr_assert(pthread_create(&threads[i], NULL, t_main, ptr) == 0);
		}

		for (int i = 0; i < tcount; i++)
		{
			/* check that thread is correctly waiting */
			cr_assert(pthread_tryjoin_np(threads[i], NULL) == EBUSY);
		}

		for (int i = 0; i < tcount; i++)
		{
			void *str = strdup("String");
			cr_assert(ptr->v->safe_add(ptr, str) == 0);
		}

		/* sleep to let threads handle items added, then check if threads
		 * are finished */
		usleep(1000);
		for (int i = 0; i < tcount; i++)
		{
			void *ret;
			cr_assert(pthread_tryjoin_np(threads[i], &ret) == 0);
			cr_assert(strcmp((char *)ret, "String") == 0);
			free(ret);
		}
		cr_assert(ptr->v->size(ptr) == 0);

		t_flush_buffer(ptr);
}
