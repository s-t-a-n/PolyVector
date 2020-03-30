/*
 * =====================================================================================
 *
 *       Filename:  vector_errors.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23-03-20 19:27:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stan Verschuuren (), sverschu@student.codam.nl
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef VECTOR_ERRORS_H
# define VECTOR_ERRORS_H

enum e_vector_error {
	VEC_FUL = 1, /* vector is full and cannot/may not grow */
	VEC_CAP = 2, /* cannot grow; abscap was reached */
	VEC_RUB = 3, /* request was out of bounds (invalid index) */
	VEC_MAL = 3, /* malloc failure */
	VEC_STB = 4  /* function is merely stub */
};

#endif
