// memory_graph.c
//
// Created on: May 5, 2020
//     Author: Jeff Manzione

#include "alloc/memory_graph/memory_graph.h"

#include <stdint.h>

#include "alloc/alloc.h"
#include "alloc/arena/arena.h"
#include "debug/debug.h"
#include "struct/map.h"
#include "struct/set.h"
#include "struct/struct_defaults.h"

// Considerably-large prime number.
#define DEFAULT_NODE_TABLE_SZ 997
#define DEFAULT_ROOT_TABLE_SZ DEFAULT_TABLE_SZ
#define DEFAULT_CHILDREN_TABLE_SZ 17

typedef Node *(*NProducer)();

struct __MGraph {
  MGraphConf config;
  __Arena node_arena;  // Node
  __Arena edge_arena;  // _Edge
  Set nodes;           // Node
  Set roots;           // Node
  uint32_t node_count;
};

typedef struct {
  uint32_t int_id;
} _Id;

struct __Node {
  _Id id;
  Ref ptr;
  Deleter del;
  Map children;  // key: Node, vale: _Edge
  Map parents;   // key: Node, vale: _Edge
};

typedef struct {
  Node *node;
  uint32_t ref_count;
} _Edge;

uint32_t _node_id(MGraph *mg);
uint32_t _node_hasher(const void *node);
int32_t _node_comparator(const void *n1, const void *n2);
uint32_t _edge_hasher(const void *node);
int32_t _edge_comparator(const void *n1, const void *n2);
Node *_node_create(MGraph *mg, Ref ptr, Deleter del);
void _node_delete(MGraph *mg, Node *node, bool delete_edges, bool delete_node);
_Edge *_edge_create(MGraph *mg, Node *node);
void _edge_delete(MGraph *mg, _Edge *edge);

MGraph *mgraph_create(const MGraphConf *const config) {
  ASSERT(NOT_NULL(config));
  MGraph *mg = ALLOC2(MGraph);
  mg->config = *config;
  __arena_init(&mg->node_arena, sizeof(Node), "Node");
  __arena_init(&mg->edge_arena, sizeof(_Edge), "_Edge");
  set_init_custom_comparator(&mg->nodes, DEFAULT_NODE_TABLE_SZ, _node_hasher,
                             _node_comparator);
  set_init_custom_comparator(&mg->roots, DEFAULT_ROOT_TABLE_SZ, _node_hasher,
                             _node_comparator);
  mg->node_count = 0;
  return mg;
}

void mgraph_delete(MGraph *mg) {
  ASSERT(NOT_NULL(mg));
  void delete_all(void *ptr) {
    _node_delete(mg, (Node *)ptr, /*delete_edges=*/true, /*delete_node=*/true);
  }
  set_iterate(&mg->nodes, delete_all);
  __arena_finalize(&mg->node_arena);
  __arena_finalize(&mg->edge_arena);
  set_finalize(&mg->nodes);
  set_finalize(&mg->roots);
  DEALLOC(mg);
}

Node *mgraph_insert(MGraph *mg, Ref ptr, Deleter del) {
  ASSERT(NOT_NULL(mg), NOT_NULL(ptr), NOT_NULL(del));
  Node *node = _node_create(mg, ptr, del);
  set_insert(&mg->nodes, node);
  return node;
}

void mgraph_root(MGraph *mg, Node *node) {
  ASSERT(NOT_NULL(mg), NOT_NULL(node));
  set_insert(&mg->roots, node);
}

void mgraph_inc(MGraph *mg, Node *parent, Node *child) {
  ASSERT(NOT_NULL(mg), NOT_NULL(parent), NOT_NULL(child));
  _Edge *p2c = map_lookup(&parent->children, child);
  if (NULL == p2c) {
    map_insert(&parent->children, child, _edge_create(mg, child));
  } else {
    p2c->ref_count++;
  }
  _Edge *c2p = map_lookup(&child->parents, parent);
  if (NULL == c2p) {
    map_insert(&child->parents, parent, _edge_create(mg, parent));
  } else {
    c2p->ref_count++;
  }
}

void mgraph_dec(MGraph *mg, Node *parent, Node *child) {
  ASSERT(NOT_NULL(mg), NOT_NULL(parent), NOT_NULL(child));
  _Edge *p2c = map_lookup(&parent->children, child);
  if (NULL == p2c) {
    ERROR("Removing reference from parent %p to %p which did not exist.",
          parent->ptr, child->ptr);
    map_insert(&parent->children, child, _edge_create(mg, child));
  }
  p2c->ref_count--;
  _Edge *c2p = map_lookup(&child->parents, parent);
  if (NULL == c2p) {
    ERROR("Removing reference from child %p to %p which did not exist.",
          child->ptr, parent->ptr);
    map_insert(&child->parents, parent, _edge_create(mg, parent));
  }
  c2p->ref_count--;
}

void mgraph_collect_garbage(MGraph *mg) {
  ASSERT(NOT_NULL(mg));
  Set marked;
  set_init_custom_comparator(&marked, set_size(&mg->nodes) * 2, _node_hasher,
                             _node_comparator);
  void mark_node(void *ptr) {
    Node *node = (Node *)ptr;
    if (!set_insert(&marked, node)) {
      // Node already processed
      return;
    }
    void mark_node_child(Pair * kv) {
      _Edge *e = (_Edge *)kv->value;
      if (e->ref_count > 0) {
        mark_node((void *)kv->key);  // blessed.
      }
    }
    map_iterate(&node->children, mark_node_child);
  }
  set_iterate(&mg->roots, mark_node);

  void delete_node_if_not_marked(void *ptr) {
    Node *node = (Node *)ptr;
    if (set_lookup(&marked, node)) {
      return;
    }
    _node_delete(mg, node, mg->config.eager_delete_edges,
                 mg->config.eager_delete_nodes);
  }
  set_iterate(&mg->nodes, delete_node_if_not_marked);
  set_finalize(&marked);
}

uint32_t _node_id(MGraph *mg) {
  ASSERT(NOT_NULL(mg));
  return mg->node_count++;
}

uint32_t _node_hasher(const void *node) {
  ASSERT(NOT_NULL(node));
  return ((Node *)node)->id.int_id;
}

int32_t _node_comparator(const void *n1, const void *n2) {
  ASSERT(NOT_NULL(n1), NOT_NULL(n2));
  return ((Node *)n1)->id.int_id - ((Node *)n2)->id.int_id;
}

uint32_t _edge_hasher(const void *node) {
  ASSERT(NOT_NULL(node));
  return ((Node *)node)->id.int_id;
}

int32_t _edge_comparator(const void *n1, const void *n2) {
  ASSERT(NOT_NULL(n1), NOT_NULL(n2));
  return ((Node *)n1)->id.int_id - ((Node *)n2)->id.int_id;
}

Node *_node_create(MGraph *mg, Ref ptr, Deleter del) {
  ASSERT(NOT_NULL(mg), NOT_NULL(ptr), NOT_NULL(del));
  Node *node = (Node *)__arena_alloc(&mg->node_arena);
  node->ptr = ptr;
  node->del = del;
  map_init_custom_comparator(&node->children, DEFAULT_CHILDREN_TABLE_SZ,
                             _node_hasher, _node_comparator);
  map_init_custom_comparator(&node->parents, DEFAULT_CHILDREN_TABLE_SZ,
                             _node_hasher, _node_comparator);
  return node;
}

void _node_delete(MGraph *mg, Node *node, bool delete_edges, bool delete_node) {
  ASSERT(NOT_NULL(mg), NOT_NULL(node));
  node->del(node->ptr);
  if (delete_edges) {
    void delete_edge(Pair * pair) {
      __arena_dealloc(&mg->edge_arena, (_Edge *)pair->value);
    }
    map_iterate(&node->children, delete_edge);
    map_finalize(&node->children);
    map_iterate(&node->parents, delete_edge);
    map_finalize(&node->parents);
  }
  if (delete_node) {
    __arena_dealloc(&mg->node_arena, node);
  }
}

_Edge *_edge_create(MGraph *mg, Node *node) {
  _Edge *edge = (_Edge *)__arena_alloc(&mg->edge_arena);
  edge->node = node;
  edge->ref_count = 1;
  return edge;
}

void _edge_delete(MGraph *mg, _Edge *edge) {
  __arena_dealloc(&mg->edge_arena, edge);
}