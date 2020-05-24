load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "map",
    srcs = ["map.c"],
    hdrs = ["map.h"],
    deps = [
        "//error",
        "//util",
    ],
)

cc_library(
    name = "set",
    srcs = ["set.c"],
    hdrs = ["set.h"],
    deps = [
        ":map",
        "//error",
        "//util",
    ],
)

cc_library(
    name = "struct_defaults",
    srcs = ["struct_defaults.c"],
    hdrs = ["struct_defaults.h"],
    deps = [
        ":map",
        ":set",
        "//alloc",
    ],
)
