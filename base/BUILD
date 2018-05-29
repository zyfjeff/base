load("//:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "base",
    srcs = [
        "token_bucket.cc",
        "token_bucket.h"
    ]
)

cc_test(
    name = "token_bucket_test",
    srcs = ["token_bucket_test.cc"],
 #   copts = TEST_COPTS,
    deps = [
        ":base",
        "@com_google_googletest//:gtest_main",
    ]
)