package(default_visibility = ["//visibility:public"])

cc_library(
    name = "kcfg",
    hdrs = [
        "kcfg.hpp",
        "kcfg_common.h",
        "kcfg_json.h",
        "kcfg_toml.h",
    ],
    includes = ["./"],
    deps = [
        "@com_github_tencent_rapidjson//:rapidjson",
        "@com_github_toml11//:toml",
    ],
)

filegroup(
    name = "kcfg_headers",
    srcs = [
        "kcfg.hpp",
        "kcfg_common.h",
        "kcfg_json.h",
        "kcfg_toml.h",
    ],
)

