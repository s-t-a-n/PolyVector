/*
 * =====================================================================================
 *
 *       Filename:  fifobuffer_t.c
 *
 *    Description:  Basic tests of fifobuffer
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

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "vector.h"
#include "types/fifobuffer.h"

int		main(void)
{
	size_t cap = 512;

	struct FiFoBuffer	*fbuf = vecnew(FiFoBuffer, cap, free, strdup);

	int error = 0;

	error += fbuf->v->push(fbuf, strdup("1"));
	error += fbuf->v->push(fbuf, strdup("2"));
	error += fbuf->v->push(fbuf, strdup("3"));
	error += fbuf->v->push(fbuf, strdup("4"));
	error += fbuf->v->push(fbuf, strdup("5"));
	error += fbuf->v->push(fbuf, strdup("6"));
	error += fbuf->v->push(fbuf, strdup("7"));
	error += fbuf->v->push(fbuf, strdup("8"));
	error += fbuf->v->push(fbuf, strdup("9"));
	error += fbuf->v->pushback(fbuf, strdup("10"));

	printf("errors : %i, size of vec : %lu\n", error, fbuf->v->size(fbuf));

	struct FiFoBuffer *clone = vecclone(fbuf);

	//fbuf->v->set(fbuf, 2, strdup("differ"));

	for (int i = 0; i < 10; i++)
	{
		char *str = clone->v->peek(clone);
		printf("clone : %s\n", str);
		clone->free(str);
		clone->v->pop(clone);
	}

	for (int i = 0; i < 5; i++)
	{
		char *str = fbuf->v->peek(fbuf);
		printf("fbuf : %s\n", str);
		fbuf->free(str);
		fbuf->v->pop(fbuf);
	}

	vecdestroy(fbuf);
	vecdestroy(clone);
	return (0);
}
