load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

_TOML11_BUILD_FILE = """
cc_library(
    name = "toml",
    hdrs = glob([
        "**/*.hpp",
    ]),
    visibility = [ "//visibility:public" ],
)
"""

toml11_ver = "3.6.1"

toml11_name = "toml11-{ver}".format(ver = toml11_ver)

http_archive(
    name = "com_github_toml11",
    build_file_content = _TOML11_BUILD_FILE,
    strip_prefix = toml11_name,
    urls = [
        "https://github.com/ToruNiina/toml11/archive/v{ver}.tar.gz".format(ver = toml11_ver),
    ],
)

_RAPIDJSON_BUILD_FILE = """
cc_library(
    name = "rapidjson",
    hdrs = glob(["include/rapidjson/**/*.h"]),
    includes = ["include"],
    defines = ["RAPIDJSON_HAS_STDSTRING=1"],
    visibility = [ "//visibility:public" ],
)
"""

rapidjson_ver = "1.1.0"

rapidjson_name = "rapidjson-{ver}".format(ver = rapidjson_ver)

http_archive(
    name = "com_github_tencent_rapidjson",
    build_file_content = _RAPIDJSON_BUILD_FILE,
    strip_prefix = rapidjson_name,
    urls = [
        "https://github.com/Tencent/rapidjson/archive/v{ver}.tar.gz".format(ver = rapidjson_ver),
    ],
)

com_github_jbeder_yaml_cpp_ver = "0.7.0"

com_github_jbeder_yaml_cpp_urls = [
    "https://github.com/jbeder/yaml-cpp/archive/yaml-cpp-{ver}.tar.gz".format(ver = com_github_jbeder_yaml_cpp_ver),
]

http_archive(
    name = "com_github_jbeder_yaml_cpp",
    strip_prefix = "yaml-cpp-yaml-cpp-{ver}".format(ver = com_github_jbeder_yaml_cpp_ver),
    urls = com_github_jbeder_yaml_cpp_urls,
)

gtest_ver = "1.14.0"

gtest_name = "googletest-{ver}".format(ver = gtest_ver)

http_archive(
    name = "com_google_googletest",
    strip_prefix = gtest_name,
    urls = [
        "https://github.com/google/googletest/archive/refs/tags/v{ver}.tar.gz".format(ver = gtest_ver),
    ],
)

# load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies")

# rules_cc_dependencies()

# Hedron's Compile Commands Extractor for Bazel
# https://github.com/hedronvision/bazel-compile-commands-extractor
http_archive(
    name = "hedron_compile_commands",
    strip_prefix = "bazel-compile-commands-extractor-0e990032f3c5a866e72615cf67e5ce22186dcb97",

    # Replace the commit hash (0e990032f3c5a866e72615cf67e5ce22186dcb97) in both places (below) with the latest (https://github.com/hedronvision/bazel-compile-commands-extractor/commits/main), rather than using the stale one here.
    # Even better, set up Renovate and let it do the work for you (see "Suggestion: Updates" in the README).
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/0e990032f3c5a866e72615cf67e5ce22186dcb97.tar.gz",
    # When you first run this tool, it'll recommend a sha256 hash to put here with a message like: "DEBUG: Rule 'hedron_compile_commands' indicated that a canonical reproducible form can be obtained by modifying arguments sha256 = ..."
)

load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")

hedron_compile_commands_setup()

load("@hedron_compile_commands//:workspace_setup_transitive.bzl", "hedron_compile_commands_setup_transitive")

hedron_compile_commands_setup_transitive()

load("@hedron_compile_commands//:workspace_setup_transitive_transitive.bzl", "hedron_compile_commands_setup_transitive_transitive")

hedron_compile_commands_setup_transitive_transitive()

load("@hedron_compile_commands//:workspace_setup_transitive_transitive_transitive.bzl", "hedron_compile_commands_setup_transitive_transitive_transitive")

hedron_compile_commands_setup_transitive_transitive_transitive()
