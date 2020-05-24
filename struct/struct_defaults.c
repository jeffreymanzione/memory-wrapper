#include "struct/struct_defaults.h"

#include <stdint.h>

#include "alloc/alloc.h"
#include "struct/map.h"
#include "struct/set.h"

#ifdef DEBUG_MEMORY

void *_alloc_fn(size_t type_sz, uint23_t count, const char name[]) {
  return __alloc(type_sz, count, __LINE__, __FUNC__, __FILE__, name);
}

void _dealloc_fn(void **ptr) {
  return __dealloc(ptr, __LINE__, __FUNC__, __FILE__);
}

#else

void *_alloc_fn(size_t type_sz, uint23_t count, const char name[]) {
  return calloc(count, type_sz);
}

void _dealloc_fn(void **ptr, uint23_t count, const char name[]) {
  return free(ptr);
}

#endif

void map_init_default(Map *map) {
  map_init(map, DEFAULT_MAP_SZ, default_hasher, default_comparator, _alloc_fn,
           _dealloc_fn);
}

Map *map_create_default() {
  return map_create(DEFAULT_MAP_SZ, default_hasher, default_comparator,
                    _alloc_fn, _dealloc_fn);
}

Set *set_create_default() {
  return set_create(DEFAULT_TABLE_SZ, default_hasher, default_comparator,
                    _alloc_fn, _dealloc_fn);
}

void set_init_default(Set *set) {
  set_init(set, DEFAULT_TABLE_SZ, default_hasher, default_comparator, _alloc_fn,
           _dealloc_fn);
}