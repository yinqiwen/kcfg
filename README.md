kcfg
======

A head-only C++ json/toml/yaml config mapping library. 

## Dependency

- [rapidjson](https://github.com/Tencent/rapidjson)
- [toml](https://github.com/ToruNiina/toml11)
- [yaml](https://github.com/jbeder/yaml-cpp)

## Limit
- C++17
- Max 256 fields in a struct/class

## Example

```cpp
#include "kcfg_json.h"
#include "kcfg_toml.h"
#include "kcfg_yaml.h"

struct ConfigItem
{
        int v0;
        int y0;
        KCFG_DEFINE_FIELDS(v0, y0);

        KCFG_YAML_DEFINE_FIELDS(v0, y0);

        KCFG_TOML_DEFINE_FIELDS(v0, y0);
};

struct Config
{
        int64_t x;
        int y;
        float z;
        std::string aa;
        std::map<std::string, int> m1;
        std::vector<std::string> v2;
        std::vector<ConfigItem> v3;

        KCFG_DEFINE_FIELDS(x, y, z, aa, m1, v2, v3);

        KCFG_YAML_DEFINE_FIELDS(x, y, z, aa, m1, v2, v3);

        KCFG_TOML_DEFINE_FIELDS(x, y, z, aa, m1, v2, v3);

        Config()
                : x(0), y(0), z(0)
        {
        }
};


int main(int argc, char *argv[])
{

    std::string json = "{\"x\":1, \"y\":101, \"z\":2.1, \"aa\":\"val\", \"m1\":{\"k1\":102}, \"v2\":[\"v0\"]}";

    Config cfg;
    kcfg::ParseFromJsonString(json, cfg);
    // kcfg::ParseFromTomlString(content, t);
    // kcfg::ParseFromYamlString(content, t);

    cfg.v2.push_back("testa");
    cfg.v2.push_back("testv");
    cfg.v3.resize(2);
    cfg.v3[0].v0 = 101;
    cfg.v3[1].y0 = 987;
    std::string kstr;
    kcfg::WriteToJsonString(cfg, kstr);
    printf("%s\n",kstr.c_str());

    return 0;
}
```

## Embedding kcfg

Just copy all headers into your project. 
