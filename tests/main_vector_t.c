/*
 * =====================================================================================
 *
 *       Filename:  vector_main_t.c
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

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "class.h"
#include "vector.h"

int	main(void)
{
	const char *str = "hallo vector";
	size_t size = strlen(str);
	size_t cap = 512;

	struct Vector	*myvec = new(Vector, str, size, cap);
	//myvec->pushback(myvec, "eeeh rustig");
	destroy(myvec);
	return (0);
}
