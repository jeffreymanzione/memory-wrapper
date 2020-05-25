/*
 * arena.h
 *
 *  Created on: Feb 7, 2018
 *      Author: Jeff
 */

#ifndef ALLOC_ARENA_ARENA_H_
#define ALLOC_ARENA_ARENA_H_

#include <stddef.h>
#ifdef DEBUG
#include <stdint.h>
#endif

#define ARENA_DECLARE(typename)       \
  extern __Arena __ARENA__##typename; \
  void *__arena_alloc__##typename()

#define ARENA_DEFINE(typename) \
  __Arena __ARENA__##typename; \
  void *__arena_alloc__##typename()

#define ARENA_INIT(typename) \
  __arena_init(&__ARENA__##typename, sizeof(typename), #typename)
#define ARENA_FINALIZE(typename) __arena_finalize(&__ARENA__##typename)
#define ARENA_ALLOC(typename) (typename *)__arena_alloc(&__ARENA__##typename)
#define ARENA_DEALLOC(typename, ptr) __arena_dealloc(&___ARENA__##typename, ptr)

typedef struct __Subarena _Subarena;

typedef struct {
  const char *name;
  _Subarena *last;
  size_t alloc_sz;
  void *next, *end;
  void *last_freed;

#ifdef DEBUG
  uint32_t requests, removes;
#endif

} __Arena;

// Do not call these function directly.
void __arena_init(__Arena *arena, size_t sz, const char name[]);
void __arena_finalize(__Arena *arena);
void *__arena_alloc(__Arena *arena);
void __arena_dealloc(__Arena *arena, void *ptr);

#endif /* ALLOC_ARENA_ARENA_H_ */
