// intern.c
//
// Created on: Feb 11, 2018
//     Author: Jeff Manzione

#include "alloc/arena/intern.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "alloc/alloc.h"
#include "debug/debug.h"
#include "struct/set.h"
#include "util/util.h"

#define DEFAULT_CHUNK_SIZE 32488
#define DEFAULT_HASHTABLE_SIZE 4091

typedef struct __Chunk _Chunk;

struct __Chunk {
  char *block;
  _Chunk *next;
  size_t sz;
};

typedef struct {
  char *tail, *end;
  _Chunk *chunk, *last;
  Set strings;
} _Strings;

static _Strings strings;

_Chunk *_chunk_create() {
  _Chunk *chunk = ALLOC2(_Chunk);
  chunk->sz = DEFAULT_CHUNK_SIZE;
  chunk->block = ALLOC_ARRAY2(char, chunk->sz);
  chunk->next = NULL;
  return chunk;
}

void _chunk_delete(_Chunk *chunk) {
  ASSERT_NOT_NULL(chunk);
  if (NULL != chunk->next) {
    _chunk_delete(chunk->next);
  }
  DEALLOC(chunk->block);
  DEALLOC(chunk);
}

void *_malloc_fn(size_t type_sz, size_t count, const char name[]) {
  return malloc(count * type_sz);
}

void _free_fn(void **ptr) { return free(*ptr); }

void intern_init() {
  strings.chunk = strings.last = _chunk_create();
  strings.tail = strings.chunk->block;
  strings.end = strings.tail + strings.chunk->sz;
  set_init(&strings.strings, DEFAULT_HASHTABLE_SIZE, string_hasher,
           string_comparator, _malloc_fn, _free_fn);
}

void intern_finalize() {
  set_finalize(&strings.strings);
  _chunk_delete(strings.chunk);
}

char *intern_range(const char str[], int start, int end) {
  char *tmp = ALLOC_ARRAY(char, end - start + 1);
  strncpy(tmp, str + start, end - start);
  tmp[end - start] = '\0';
  char *to_return = intern(tmp);
  DEALLOC(tmp);
  return to_return;
}

char *intern(const char str[]) {
  char *str_lookup = (char *)set_lookup(&strings.strings, str);
  if (NULL != str_lookup) {
    return str_lookup;
  }
  uint32_t len = strlen(str);
  if (strings.tail + len >= strings.end) {
    strings.last->next = _chunk_create();
    strings.last = strings.last->next;
    strings.tail = strings.last->block;
    strings.end = strings.tail + strings.last->sz;
  }
  char *to_return = strings.tail;
  memmove(strings.tail, str, len + 1);
  strings.tail += (len + 1);
  set_insert(&strings.strings, to_return);
  return to_return;
}
