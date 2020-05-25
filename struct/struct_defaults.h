#ifndef STRUCT_STRUCT_DEFAULTS_H_
#define STRUCT_STRUCT_DEFAULTS_H_

#include "struct/map.h"
#include "struct/set.h"

// Initializes a Map and allocates the internal memory for it with the default
// setup.
//
// Details:
//   - Assumes that the [map] is allocated on the stack as it does not allocate
//     the memory for the Map struct.
//   - The hasher and comparator rely on pointer difference.
//
// Usage:
//   Map map;
//   map_init_default(&map);
void map_init_default(Map *map);

// Creates a Map and allocates the memory for it, returning a pointer to that
// map with the default setup.
//
// Usage:
//   Map *map = map_create_default();
Map *map_create_default();

// Initializes a Set and allocates the internal memory for it with the default
// setup.
//
// Details:
//   - Assumes that the [set] is allocated on the stack as it does not allocate
//     the memory for the Set struct.
//   - The hasher and comparator rely on pointer difference.
//
// Usage:
//   Set set;
//   set_init_default(&set);
void set_init_default(Set *set);

// Creates a Set and allocates the memory for it, returning a pointer to that
// map with the default setup.
//
// Usage:
//   Set *set = set_create_default();
Set *set_create_default();

// Functions that can be used to satisfy Alloc and Dealloc interfaces where
// ALLOC() and DEALLOC() are not possible.
void *__malloc_fn(size_t type_sz, size_t count, const char name[]);
void *__calloc_fn(size_t type_sz, size_t count, const char name[]);
void __free_fn(void **ptr);

#endif /* STRUCT_STRUCT_DEFAULTS_H_ */