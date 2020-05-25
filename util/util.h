/*
 * shared.h
 *
 *  Created on: Jun 17, 2017
 *      Author: Jeff
 */

#ifndef SHARED_H_
#define SHARED_H_

#include <stdint.h>
#include <stdio.h>

/*
 * Function which takes a void pointer and returns an uint32_t set value for
 * for it.
 */
typedef uint32_t (*Hasher)(const void *);
typedef int32_t (*Comparator)(const void *, const void *);

uint32_t default_hasher(const void *ptr);
int32_t default_comparator(const void *ptr1, const void *ptr2);
uint32_t string_hasher(const void *ptr);
int32_t string_comparator(const void *ptr1, const void *ptr2);
uint32_t string_hasher_len(const char *ptr, size_t len);

#endif /* SHARED_H_ */
