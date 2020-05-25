

#include <stdint.h>
#include <stdio.h>

#include "alloc/alloc.h"
#include "alloc/arena/arena.h"
#include "alloc/arena/intern.h"
#include "struct/map.h"
#include "struct/set.h"
#include "struct/struct_defaults.h"

typedef struct {
  uint16_t some_uint16;
  uint32_t some_uint32;
  double some_float64;
} TestStruct;

ARENA_DEFINE(TestStruct);

int main(int argc, const char *argv[]) {
  alloc_init();
  alloc_set_verbose(true);
  intern_init();

  ARENA_INIT(TestStruct);

  TestStruct *test = ALLOC(TestStruct);
  test->some_uint16 = 666;
  test->some_uint32 = 66666666;
  test->some_float64 = .666666;

  printf("%d %d %f\n", test->some_uint16, test->some_uint32,
         test->some_float64);

  DEALLOC(test);

  TestStruct *test_arena = ARENA_ALLOC(TestStruct);
  test_arena->some_uint16 = 777;
  test_arena->some_uint32 = 77777777;
  test_arena->some_float64 = .777777;
  printf("%d %d %f\n", test_arena->some_uint16, test_arena->some_uint32,
         test_arena->some_float64);

  Map *map = map_create_default();
  int abc = 123, def = 456, ghi = 789;
  map_insert(map, "abc", &abc);
  map_insert(map, "def", &def);
  map_insert(map, "ghi", &ghi);

  printf("%s=%d\n", "abc", *((int *)map_lookup(map, "abc")));
  printf("%s=%d\n", "def", *((int *)map_lookup(map, "def")));
  printf("%s=%d\n", "ghi", *((int *)map_lookup(map, "ghi")));

  map_delete(map);

  Set *set = set_create_default();
  set_insert(set, &abc);
  set_insert(set, &def);
  set_insert(set, &ghi);

  printf("%s=%d\n", "abc", *((int *)set_lookup(set, &abc)));
  printf("%s=%d\n", "def", *((int *)set_lookup(set, &def)));
  printf("%s=%d\n", "ghi", *((int *)set_lookup(set, &ghi)));
  set_delete(set);

  char *str = intern("Hello, world!\n");
  printf("%s", str);

  ARENA_FINALIZE(TestStruct);

  intern_finalize();
  alloc_finalize();

  return EXIT_SUCCESS;
}