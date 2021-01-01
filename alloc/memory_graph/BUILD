load("@rules_cc//cc:defs.bzl", "cc_library")

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
)
