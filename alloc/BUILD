load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "alloc",
    srcs = ["alloc.c"],
    hdrs = ["alloc.h"],
    deps = [
        "//debug",
        "//struct:set",
    ],
    alwayslink = True,
)
