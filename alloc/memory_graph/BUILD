load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "memory_graph",
    srcs = ["memory_graph.c"],
    hdrs = ["memory_graph.h"],
    deps = [
        "//alloc",
        "//alloc/arena",
        "//debug",
        "//struct:map",
        "//struct:set",
        "//struct:struct_defaults",
    ],
    alwayslink = True,
)

cc_test(
    name = "memory_graph_test",
    srcs = ["test/memory_graph_test.cc"],
    linkstatic = True,
    deps = [
        ":memory_graph",
        "//alloc",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
