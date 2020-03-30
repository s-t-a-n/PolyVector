/*
 * =====================================================================================
 *
 *       Filename:  main_buffer_t.c
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
#include "types/buffer.h"

int		main(void)
{
	size_t cap = 2;
	size_t abscap = 12;

	struct Buffer	*buf = vecnew(Buffer, cap, abscap, free, strdup);

	int error = 0;

	for (int i = 0; i < 10; i++)
	{
		char c[10];

		snprintf(c, 10, "%i", i);
		void *ptr = strdup(c);

		error = buf->v->push(buf, ptr);

		if (error != 0)
		{
			printf("pushed obj |%s| with error %i\n", c, error);
			free(ptr);
		}
	}
	//error += buf->v->pushback(buf, strdup("pushback"));

	printf("errors : %i, size of buf : %lu\n", error, buf->v->size(buf));

	struct Buffer *clone = vecclone(buf);

	void *ptr;

	ptr = strdup("differ");
	error = buf->v->set(buf, 8, ptr);
	if (error != 0)
			free(ptr);
	ptr = strdup("insert");
	error += clone->v->insert(clone, 2, ptr);
	if (error != 0)
			free(ptr);

	clone->v->remove(clone, 5);

	printf("errors : %i, size of clone : %lu\n", error, clone->v->size(clone));
	for (int i = 0; i < 10; i++)
	{
		char *str = clone->v->peek(clone);
		printf("clone : %s\n", str);
		clone->free(str);
		clone->v->pop(clone);
	}

	for (int i = 0; i < 5; i++)
	{
		char *str = buf->v->peek(buf);
		printf("buf : %s\n", str);
		buf->free(str);
		buf->v->pop(buf);
	}

	vecdestroy(buf);
	vecdestroy(clone);
	return (0);
}
