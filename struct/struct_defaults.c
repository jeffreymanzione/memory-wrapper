// struct_defaults.c
//
// Created on: Feb 6, 2018
//     Author: Jeff Manzione

#include "struct/struct_defaults.h"

#include <stdint.h>

#include "alloc/alloc.h"
#include "struct/map.h"
#include "struct/set.h"

void map_init_default(Map *map) {
  map_init(map, DEFAULT_MAP_SZ, default_hasher, default_comparator, __calloc_fn,
           __free_fn);
}

Map *map_create_default() {
  return map_create(DEFAULT_MAP_SZ, default_hasher, default_comparator,
                    __calloc_fn, __free_fn);
}

Set *set_create_default() {
  return set_create(DEFAULT_TABLE_SZ, default_hasher, default_comparator,
                    __calloc_fn, __free_fn);
}

void set_init_default(Set *set) {
  set_init(set, DEFAULT_TABLE_SZ, default_hasher, default_comparator,
           __calloc_fn, __free_fn);
}

#ifdef DEBUG_MEMORY

void *__malloc_fn(size_t type_sz, size_t count, const char name[]) {
  return __alloc(type_sz, count, __LINE__, __func__, __FILE__, name);
}

void *__calloc_fn(size_t type_sz, size_t count, const char name[]) {
  return __alloc(type_sz, count, __LINE__, __func__, __FILE__, name);
}

void __free_fn(void **ptr) {
  return __dealloc(ptr, __LINE__, __func__, __FILE__);
}

#else

void *__malloc_fn(size_t type_sz, size_t count, const char name[]) {
  return malloc(count * type_sz);
}

void *__calloc_fn(size_t type_sz, size_t count, const char name[]) {
  return calloc(count, type_sz);
}

void __free_fn(void **ptr) { return free(*ptr); }

#endif