/*
 *Copyright (c) 2017-2017, yinqiwen <yinqiwen@gmail.com>
 *All rights reserved.
 *
 *Redistribution and use in source and binary forms, with or without
 *modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of Redis nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 *THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 *BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 *THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "kcfg.hpp"

struct ConfigItem
{
        int v0;
        int y0;
        KCFG_DEFINE_FIELDS(v0, y0);
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
};


int main(int argc, char *argv[])
{

    std::string json = "{\"x\":1, \"y\":101, \"z\":2.1, \"aa\":\"val\", \"m1\":{\"k1\":102}, \"v2\":[\"v0\"]}";

    Config cfg;
    kcfg::ParseFromJsonString(json, cfg);

    cfg.v2.push_back("testa");
    cfg.v2.push_back("testv");
    cfg.v3.resize(2);
    cfg.v3[0].v0 = 101;
    cfg.v3[1].y0 = 987;
    std::string kstr;
    kcfg::WriteToJsonString(cfg, kstr);
    printf("%s\n",kstr.c_str());

//    std::string macros;
//    for( int i = 1; i < 64; i++)
//    {
//        char buffer[1024];
//        sprintf(buffer, "#define KCFG_FOR_EACH_%d(what, x, ...)\\", i+1);
//        macros.append(buffer).append("\n");
//        macros.append("  what(x);\\").append("\n");
//        sprintf(buffer, "  KCFG_FOR_EACH_%d(what,  __VA_ARGS__)", i);
//        macros.append(buffer).append("\n");
//    }
//
//    printf("%s\n",macros.c_str());

    return 0;
}

