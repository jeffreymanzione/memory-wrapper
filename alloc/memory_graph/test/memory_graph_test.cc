#include "gtest/gtest.h"

extern "C" {
#include "alloc/alloc.h"
#include "alloc/memory_graph/memory_graph.h"
#include "debug/debug.h"
#include "stdbool.h"
}

namespace {
void noop(void *) {}
}  // namespace

TEST(memory_graph, create_and_delete) {
  alloc_init();
  MGraphConf config = {.eager_delete_edges = true, .eager_delete_nodes = true};
  MGraph *mg = mgraph_create(&config);
  mgraph_delete(mg);
  alloc_finalize();
}

TEST(memory_graph, adds_items) {
  alloc_init();
  MGraphConf config = {.eager_delete_edges = true, .eager_delete_nodes = true};
  MGraph *mg = mgraph_create(&config);

  char p1[] = "Test1";
  char p2[] = "Test2";

  mgraph_insert(mg, p1, noop);
  mgraph_insert(mg, p2, noop);

  mgraph_delete(mg);
  alloc_finalize();
}