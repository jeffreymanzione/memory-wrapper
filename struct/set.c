// set.c
//
// Created on: Feb 6, 2018
//     Author: Jeff Manzione

#include "struct/set.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "debug/debug.h"
#include "struct/map.h"
#include "util/util.h"

Set *set_create(uint32_t size, Hasher hasher, Comparator comparator,
                Alloc alloc, Dealloc dealloc) {
  Set *set = (Set *)alloc(sizeof(Set), 1, "Set");
  set_init(set, size, hasher, comparator, alloc, dealloc);
  return set;
}

inline void set_init(Set *set, uint32_t size, Hasher hasher,
                     Comparator comparator, Alloc alloc, Dealloc dealloc) {
  map_init(&set->map, size, hasher, comparator, alloc, dealloc);
}

inline void set_finalize(Set *set) {
  ASSERT_NOT_NULL(set);
  map_finalize(&set->map);
}

inline void set_delete(Set *set) {
  ASSERT_NOT_NULL(set);
  set_finalize(set);
  set->map.dealloc((void **)&set);
}

inline bool set_insert(Set *set, const void *ptr) {
  ASSERT_NOT_NULL(set);
  return map_insert(&set->map, ptr, ptr);
}

inline bool set_remove(Set *set, const void *ptr) {
  ASSERT_NOT_NULL(set);
  Pair p = map_remove(&set->map, ptr);
  return NULL != p.value;
}

inline void *set_lookup(const Set *set, const void *ptr) {
  ASSERT_NOT_NULL(set);
  return map_lookup(&set->map, ptr);
}

inline int set_size(const Set *set) { return map_size(&set->map); }

void set_iterate(const Set *set, Action action) {
  ASSERT_NOT_NULL(set);
  M_iter iter;
  for (iter = set_iter((Set *)set); has(&iter); inc(&iter)) {
    action(value(&iter));
  }
}

inline M_iter set_iter(Set *set) { return map_iter(&set->map); }