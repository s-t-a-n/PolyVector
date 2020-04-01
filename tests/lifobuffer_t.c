/*
 * =====================================================================================
 *
 *       Filename:  lifobuffer_t.c
 *
 *    Description:  Basic tests of lifobuffer
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
#include "types/lifobuffer.h"

int		main(void)
{
	size_t cap = 512;

	struct LiFoBuffer	*lbuf = vecnew(LiFoBuffer, cap, free, strdup);

	int error = 0;

	error += lbuf->v->push(lbuf, strdup("1"));
	error += lbuf->v->push(lbuf, strdup("2"));
	error += lbuf->v->push(lbuf, strdup("3"));
	error += lbuf->v->push(lbuf, strdup("4"));
	error += lbuf->v->push(lbuf, strdup("5"));
	error += lbuf->v->push(lbuf, strdup("6"));
	error += lbuf->v->push(lbuf, strdup("7"));
	error += lbuf->v->push(lbuf, strdup("8"));
	error += lbuf->v->push(lbuf, strdup("9"));
	error += lbuf->v->pushback(lbuf, strdup("10"));

	printf("errors : %i, size of vec : %lu\n", error, lbuf->v->size(lbuf));

	struct LiFoBuffer *clone = vecclone(lbuf);

	//lbuf->v->set(lbuf, 2, strdup("differ"));

	for (int i = 0; i < 10; i++)
	{
		char *str = clone->v->peek(clone);
		printf("clone : %s\n", str);
		clone->free(str);
		clone->v->pop(clone);
	}

	for (int i = 0; i < 5; i++)
	{
		char *str = lbuf->v->peek(lbuf);
		printf("lbuf : %s\n", str);
		lbuf->free(str);
		lbuf->v->pop(lbuf);
	}

	vecdestroy(lbuf);
	vecdestroy(clone);
	return (0);
}
