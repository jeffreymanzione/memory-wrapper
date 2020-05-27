// map.h
//
// Created on: Jul 7, 2016
//     Author: Jeff Manzione

#ifndef MAP_H_
#define MAP_H_

#include <stdbool.h>
#include <stdint.h>

#include "util/util.h"

#define DEFAULT_MAP_SZ 31

// Generic functions for allocating memory.
typedef void *(*Alloc)(size_t elt_size, size_t count, const char name[]);
typedef void (*Dealloc)(void **ptr);

// A map entry.
typedef struct __Entry _Entry;

// Represents a key-value pair mapping.
typedef struct {
  const void *key;
  void *value;
} Pair;

typedef struct {
  Hasher hash;
  Comparator compare;
  Alloc alloc;
  Dealloc dealloc;
  uint32_t table_sz, num_entries, entries_thresh;
  _Entry *table, *first, *last;
} Map;

// A function which processes a Pair ptr and has no return value.
typedef void (*PairAction)(Pair *kv);

// Initializes a map and allocates the internal memory for it.
//
// Details:
//   - Assumes that the [map] is allocated on the stack as it does not allocate
//     the memory for the Map struct.
//
// Usage:
//   Map map;
//   map_init(&map, 51, my_hasher, my_comparator, my_alloc, my_dealloc);
void map_init(Map *map, uint32_t size, Hasher, Comparator, Alloc, Dealloc);

// Creates a map and allocates the memory for it, returning a pointer to that
// map.
//
// Usage:
//   Map *map = map_create(51, my_hasher, my_comparator, my_alloc, my_dealloc);
Map *map_create(uint32_t size, Hasher, Comparator, Alloc, Dealloc);

// Frees all internal memory for the Map.
//
// Details:
//   - Can only be used on maps created by map_init().
//   - Does not free the memory of items.
//
// Usage:
//   Map map;
//   map_init(&map, 51, my_hasher, my_comparator, my_alloc, my_dealloc);
//   map_finalize(&map);
void map_finalize(Map *);

// Frees a Map and its internal memory.
//
// Details:
//   - Can only be used on maps created by map_create().
//   - Does not free the memory of items.
//
// Usage:
//   Map *map = map_create(51, my_hasher, my_comparator, my_alloc, my_dealloc);
//   map_delete(map);
void map_delete(Map *);

// Inserts a [key]-[value] pair into [map] and returns true if the key-value was
// updated by this insertion.
//
// Usage:
//   Map *map = map_create(51, my_hasher, my_comparator, my_alloc, my_dealloc);
//   map_insert(map, some_key_ptr, some_value_ptr);
bool map_insert(Map *, const void *key, const void *value);

// Removes a [key] and its associated value from [map], returning a Pair with
// the key-value that was removed.
//
// Details:
//   - If the key is not present in the map, then the returned Pair.value will
//     be NULL.
//
// Usage:
//   Map *map = ...;
//   ...
//   Pair removed = map_remove(map, some_key_ptr);
Pair map_remove(Map *, const void *key);

// Returns the associated value associated with [key] stored in [map].
//
// Details:
//   - If the key is not present in the map, then NULL is returned.
//
// Usage:
//   Map *map = ...;
//   ...
//   void *val = map_lookup(map, some_key_ptr);
void *map_lookup(const Map *map, const void *key);

// Iterates through each entry in [map], applying [pair_action] to each in
// insertion order.
//
// Usage:
//   Map *map = ...;
//   ...
//   void each(Pair *kv) {
//     do_something(kv);
//   }
//   map_iterate(map, each);
void map_iterate(const Map *map, PairAction pair_action);

// Returns the number of entries in [map].
//
// Details:
//   - Does not return the size of the hash table, which is likely to be much
//     larger.
uint32_t map_size(const Map *);

#endif /* MAP_H_ */
