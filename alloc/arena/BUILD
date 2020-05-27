load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "arena",
    srcs = ["arena.c"],
    hdrs = ["arena.h"],
    deps = [
        "//alloc",
        "//debug",
    ],
)

cc_library(
    name = "intern",
    srcs = ["intern.c"],
    hdrs = ["intern.h"],
    deps = [
        "//alloc",
        "//debug",
        "//struct:set",
        "//util",
    ],
)
