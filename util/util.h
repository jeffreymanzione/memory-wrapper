// util.h
//
// Created on: Jun 17, 2017
//     Author: Jeff Manzione

#ifndef SHARED_H_
#define SHARED_H_

#include <stdint.h>
#include <stdio.h>

// Converts a void pointer into a unsigned integer to be used as a hash.
typedef uint32_t (*Hasher)(const void *);

// Compares two void pointers for order, returning a signed integer indicating
// the relative order of the two pointer inputs.
//
// Return values:
//   > 0: ptr1 goes after ptr2
//   < 0: ptr1 goes after ptr2
//   = 0: there is no relative order between the inputs.
//
// The magnitude of the return value does not necessarily provide any signal.
typedef int32_t (*Comparator)(const void *ptr1, const void *ptr2);

// Convets the ptr into an unsigned integer. The value is generally equivent to
// the input pointer, but not guaranteed.
//
// This should only be used when hashing ptrs in a context where inputs are
// unique.
uint32_t default_hasher(const void *ptr);

// Compares based on pointer math. Roughly equivalent to [ptr1 - ptr2].
//
// This should only be used when comparing ptrs in acontest where inputs are
// unique.
int32_t default_comparator(const void *ptr1, const void *ptr2);

// Does hashing and comparing based on string content.
uint32_t string_hasher(const void *ptr);
int32_t string_comparator(const void *ptr1, const void *ptr2);
uint32_t string_hasher_len(const char *ptr, size_t len);

#endif /* SHARED_H_ */
