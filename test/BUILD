load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "test",
    srcs = ["test.c"],
    deps = [
        "//alloc",
        "//alloc/arena",
        "//alloc/arena:intern",
        "//debug",
        "//struct:map",
        "//struct:set",
        "//struct:struct_defaults",
    ],
)
