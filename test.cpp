/*
 * test.cpp
 *
 *  Created on: 2017Äê12ÔÂ14ÈÕ
 *      Author: qiyingwang
 */
#include "kcfg.hpp"
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

struct ConfigItem
{
        int v0;
        int y0;
        KCFG_DEFINE_FIELDS(v0);
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
        KCFG_DEFINE_FIELDS(x, y, z, aa, m1, v2, v3)
        Config()
                : x(0), y(0), z(0)
        {
        }
        std::string ToString()
        {
            std::stringstream str;
            str << "x=" << x << ", y=" << y << ", z=" << z << ", aa=" << aa.c_str();
            str << ", v2=[";
            for (int i = 0; i < v2.size(); i++)
            {
                str << v2[i];
                if (i != v2.size() - 1)
                {
                    str << ",";
                }
            }
            str << "], ";
            str << "m1={";
            auto iter = m1.begin();
            while (iter != m1.end())
            {
                if (iter != m1.begin())
                {
                    str << ",";
                }
                str << iter->first << "->" << iter->second;
                iter++;
            }
            str << "}";
            str << "\n";
            return str.str();
        }
};

//#include <stdio.h>
//#include <stddef.h>
//
//struct a
//{
//  int a;
//  int b;
//  int c;
//};
//

//#include <stdio.h>

//
//#define PRN_STRUCT_OFFSETS_(structure, field) printf(STRINGIZE(structure)":"STRINGIZE(field)" - offset = %d\n", offsetof(structure, field));
//#define PRN_STRUCT_OFFSETS(field) PRN_STRUCT_OFFSETS_(struct a, field)
//#define PRN_FIELD(field)  printf("%s\n", #field)


int main(int argc, char *argv[])
{

//  FOR_EACH(PRN_FIELD,  a, b, aa);
    //printf("%d\n", __NARG__(1,2,3,4,5, 7,8, a, b, aa));
    std::string json = "{\"x\":1, \"y\":101, \"z\":2.1, \"aa\":\"val\", \"m1\":{\"k1\":102}, \"v2\":[\"v0\"]}";
    rapidjson::Document d;
    d.Parse<0>(json.c_str());
    if (d.HasParseError())
    {
        return -1;
    }
    Config cfg;
    cfg.PraseFromJson(d);
    printf("%s\n", cfg.ToString().c_str());

    cfg.v2.push_back("testa");
    cfg.v2.push_back("testv");
    cfg.v3.resize(2);
    cfg.v3[0].v0 = 101;
    cfg.v3[1].y0 = 987;
    rapidjson::Value tmp(rapidjson::kObjectType);
    cfg.WriteToJson(tmp);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer < rapidjson::StringBuffer > writer(buffer);
    tmp.Accept(writer);
    std::string kstr;
    kstr.append(buffer.GetString(), buffer.GetSize());
    printf("%s\n",kstr.c_str());

    return 0;
}

