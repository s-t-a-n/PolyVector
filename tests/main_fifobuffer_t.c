/*
 * =====================================================================================
 *
 *       Filename:  main_fifobuffer_t.c
 *
 *    Description:  
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
#include "fifo_buffer.h"

int	main(void)
{
	size_t cap = 512;

	struct FiFoBuffer	*fbuf = vecnew(FiFoBuffer, cap);

	int error = 0;

	error += fbuf->v->pushback(fbuf, "1");
	error += fbuf->v->pushback(fbuf, "2");
	error += fbuf->v->pushback(fbuf, "3");
	error += fbuf->v->pushback(fbuf, "4");

	printf("errors : %i\n", error);

	printf("item : %s\n", fbuf->v->peek(fbuf));
	fbuf->v->pop(fbuf);

	printf("item : %s\n", fbuf->v->peek(fbuf));
	fbuf->v->pop(fbuf);

	printf("item : %s\n", fbuf->v->peek(fbuf));
	fbuf->v->pop(fbuf);

	printf("item : %s\n", fbuf->v->peek(fbuf));
	fbuf->v->pop(fbuf);

	printf("item : %s\n", fbuf->v->peek(fbuf));
	fbuf->v->pop(fbuf);

	vecdestroy(fbuf);
	return (0);
}
