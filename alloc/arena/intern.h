/*
 * intern.h
 *
 *  Created on: Feb 11, 2018
 *      Author: Jeff
 */

#ifndef ALLOC_ARENA_INTERN_H_
#define ALLOC_ARENA_INTERN_H_

void intern_init();
void intern_finalize();
char *intern_range(const char str[], int start, int end);
char *intern(const char str[]);

#endif /* ALLOC_ARENA_INTERN_H_ */
