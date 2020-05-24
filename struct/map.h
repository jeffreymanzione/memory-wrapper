/*
 * map.h
 *
 *  Created on: Jul 7, 2016
 *      Author: Jeff
 */

#ifndef MAP_H_
#define MAP_H_

#include <stdbool.h>
#include <stdint.h>

#include "util/util.h"

#define DEFAULT_MAP_SZ 31

typedef struct __Entry _Entry;

typedef struct Pair {
  const void *key;
  void *value;
} Pair;

typedef struct _Map {
  Hasher hash;
  Comparator compare;
  Alloc alloc;
  Dealloc dealloc;
  uint32_t table_sz, num_entries, entries_thresh;
  _Entry *table, *first, *last;
} Map;

typedef void (*PairAction)(Pair *kv);

Map *map_create(uint32_t size, Hasher, Comparator, Alloc, Dealloc);
void map_init(Map *map, uint32_t size, Hasher, Comparator, Alloc, Dealloc);

void map_finalize(Map *);
void map_delete(Map *);

bool map_insert(Map *, const void *key, const void *value);
Pair map_remove(Map *, const void *key);

void *map_lookup(const Map *map, const void *key);

void map_iterate(const Map *map, PairAction pair_action);

uint32_t map_size(const Map *);

#endif /* MAP_H_ */
