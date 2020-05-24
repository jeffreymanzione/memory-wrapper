/*
 * shared.h
 *
 *  Created on: Sep 28, 2016
 *      Author: Jeff
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef DEBUG_MEMORY
#define ALLOC_ARRAY(type, count)                                        \
  (type *)__alloc(/*type=*/sizeof(type), /*count=*/(count), (__LINE__), \
                  (__func__), (__FILE__), (#type))
#define ALLOC_ARRAY2(type, count)                                       \
  (type *)__alloc(/*type=*/sizeof(type), /*count=*/(count), (__LINE__), \
                  (__func__), (__FILE__), (#type))

#define REALLOC_SZ(ptr, type_sz, count)                                   \
  (void *)__realloc(/*ptr=*/(ptr), /*type=*/(type_sz), /*count=*/(count), \
                    (__LINE__), (__func__), (__FILE__))
#define REALLOC(ptr, type, count) \
  (type *)REALLOC_SZ((ptr), sizeof(type), (count))
#define DEALLOC(ptr) \
  __dealloc((void **)&(ptr), (__LINE__), (__func__), (__FILE__))

void *__alloc(uint32_t elt_size, uint32_t count, uint32_t line,
              const char func[], const char file[], const char type_name[]);
void *__realloc(void *, uint32_t elt_size, uint32_t count, uint32_t line,
                const char func[], const char file[]);
void __dealloc(void **, uint32_t line, const char func[], const char file[]);
void alloc_finalize();

#else
#define ALLOC_ARRAY(type, count) (type *)calloc((count), sizeof(type))
#define ALLOC_ARRAY2(type, count) (type *)malloc((count) * sizeof(type))
#define REALLOC_SZ(ptr, type_sz, count) \
  (void *)realloc((ptr), (type_sz) * (count))
#define REALLOC(ptr, type, count) (type *)realloc((ptr), sizeof(type) * (count))
#define DEALLOC(ptr) free((void *)(ptr))
#endif

#define ALLOC(type) ALLOC_ARRAY(type, 1)
#define ALLOC2(type) ALLOC_ARRAY2(type, 1)

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

void alloc_init();
void alloc_set_verbose(bool);

#endif /* MEMORY_H_ */
