/*
 * set.h
 *
 *  Created on: Jul 6, 2016
 *      Author: Jeff
 */

#ifndef STRUCT_SET_H_
#define STRUCT_SET_H_

#include <stdbool.h>
#include <stdint.h>

#include "struct/map.h"
#include "util/util.h"

#define DEFAULT_TABLE_SZ DEFAULT_MAP_SZ

// A function which processes a void ptr with no return value.
typedef void (*Action)(void *ptr);

typedef struct Set_ {
  Map map;
} Set;

// Initializes a set and allocates the internal memory for it.
//
// Details:
//   - Assumes that the [set] is allocated on the stack as it does not allocate
//     the memory for the Set struct.
//
// Usage:
//   Set set;
//   set_init(&set, 51, my_hasher, my_comparator, my_alloc, my_dealloc);
void set_init(Set *set, uint32_t table_size, Hasher, Comparator, Alloc,
              Dealloc);

// Creates a set and allocates the memory for it, returning a pointer to that
// set.
//
// Usage:
//   Set *set = set_create(51, my_hasher, my_comparator, my_alloc, my_dealloc);
Set *set_create(uint32_t table_size, Hasher, Comparator, Alloc, Dealloc);

// Frees all internal memory for the Set.
//
// Details:
//   - Can only be used on sets created by set_init().
//   - Does not free the memory of items.
//
// Usage:
//   Set set;
//   set_init(&set, 51, my_hasher, my_comparator, my_alloc, my_dealloc);
//   set_finalize(&set);
void set_finalize(Set *set);

// Frees a Set and its internal memory.
//
// Details:
//   - Can only be used on sets created by set_create().
//   - Does not free the memory of items.
//
// Usage:
//   Set *set = set_create(51, my_hasher, my_comparator, my_alloc, my_dealloc);
//   set_delete(set);
void set_delete(Set *set);

// Inserts a value into [set] and returns true if the key-value was
// updated by this insertion.
//
// Usage:
//   Set *set = set_create(51, my_hasher, my_comparator, my_alloc, my_dealloc);
//   set_insert(map, some_value_ptr);
bool set_insert(Set *set, const void *value);

// Removes a value and its associated value from [set], returning a pointer to
// the value that was removed.
//
// Details:
//   - If the value is not present in the map, then NULL is returned.
//
// Usage:
//   Set *set = ...;
//   ...
//   void *val = set_remove(set, some_value_ptr);
bool set_remove(Set *set, const void *value);

// Returns the associated value associated with [value] stored in [set].
//
// Details:
//   - If the value is not present in the set, then NULL is returned.
//
// Usage:
//   Set *set = ...;
//   ...
//   void *val = set_lookup(set, some_value_ptr);
void *set_lookup(const Set *set, const void *value);

// Iterates through each item in [set], applying [action] to each in
// insertion order.
//
// Usage:
//   Set *set = ...;
//   ...
//   void each(void *val) {
//     do_something(val);
//   }
//   set_iterate(set, each);
void set_iterate(const Set *set, Action action);

// Returns the number of entries in [set].
//
// Details:
//   - Does not return the size of the hash table, which is likely to be much
//     larger.
int set_size(const Set *set);

#endif /* STRUCT_SET_H_ */
