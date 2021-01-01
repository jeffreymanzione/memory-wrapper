// util.c
//
// Created on: Jun 17, 2017
//     Author: Jeff Manzione

#include "util/util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
#define FNV_32_PRIME (0x01000193)
#define FNV_1A_32_OFFSET (0x811C9DC5)

// Is true if any bytes in the provided 32-bit integer are equivalent to 0x00.
#define HAS_NULL(x)                                      \
  (((x & 0x000000FF) == 0) || ((x & 0x0000FF00) == 0) || \
   ((x & 0x00FF0000) == 0) || ((x & 0xFF000000) == 0))

uint32_t default_hasher(const void *ptr) { return (uint32_t)(intptr_t)ptr; }

int32_t default_comparator(const void *ptr1, const void *ptr2) {
  return ((intptr_t)ptr1) - ((intptr_t)ptr2);
}

uint32_t string_hasher(const void *ptr) {
  unsigned char *s = (unsigned char *)ptr;
  uint32_t hval = FNV_1A_32_OFFSET;
  while (*s) {
    hval *= FNV_32_PRIME;
    hval ^= (uint32_t)*s++;
  }
  return hval;
}

uint32_t string_hasher_len(const char *ptr, size_t len) {
  int i;
  uint32_t hval = FNV_1A_32_OFFSET;
  for (i = 0; i < len; ++i) {
    hval *= FNV_32_PRIME;
    hval ^= (uint32_t)ptr[i];
  }
  return hval;
}

int32_t string_comparator(const void *ptr1, const void *ptr2) {
  if (ptr1 == ptr2) {
    return 0;
  }
  if (NULL == ptr1) {
    return -1;
  }
  if (NULL == ptr2) {
    return 1;
  }
  uint32_t *lhs = (uint32_t *)ptr1;
  uint32_t *rhs = (uint32_t *)ptr2;

  while (!HAS_NULL(*lhs) && !HAS_NULL(*rhs)) {
    // Longer strings will yeild positive #.
    uint32_t diff = *lhs - *rhs;
    if (diff) {
      return diff;
    }
    lhs++;
    rhs++;
  }
  return strncmp((char *)lhs, (char *)rhs, sizeof(uint32_t));
}