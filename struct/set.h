/*
 * set.h
 *
 *  Created on: Jul 6, 2016
 *      Author: Jeff
 */

#ifndef SET_H_
#define SET_H_

#include <stdbool.h>
#include <stdint.h>

#include "struct/map.h"
#include "util/util.h"

#define DEFAULT_TABLE_SZ DEFAULT_MAP_SZ

typedef struct Set_ {
  Map map;
} Set;

/*
 * Creates a new set.
 * Returns a pointer to a newly allocated Set
 */
Set *set_create(uint32_t table_size, Hasher, Comparator, Alloc, Dealloc);
void set_init(Set *set, uint32_t table_size, Hasher, Comparator, Alloc,
              Dealloc);
/*
 * Deletes a set. Frees the memory used by the set.
 */
void set_delete(Set *);
void set_finalize(Set *);

/*
 * Inserts the pointer into the set.
 * Returns true (1) if the pointer was successfully inserted
 * Returns false (0) if the pointer was already present in the set.
 */
bool set_insert(Set *, const void *);
bool set_remove(Set *, const void *);
/*
 * Checks if the value is in the set.
 * Returns the pointer (1) if the pointer is in the set,
 * Returns NULL (0) otherwise.
 */
void *set_lookup(const Set *, const void *);

/* Gets the size of a set. */
int set_size(const Set *);

/* Iterate over a set and perform the specified action on each element. */
void set_iterate(const Set *, Action);

#endif /* SET_H_ */
