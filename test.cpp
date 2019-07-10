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
        int y0;KCFG_DEFINE_FIELDS(v0, y0)
        ;
};

struct Config
{
        int64_t x;
        float y;
        float z;
        std::string aa;
        std::map<std::string, int> m1;
        std::vector<std::string> v2;
        std::vector<ConfigItem> v3;KCFG_DEFINE_FIELDS(x, y, z, aa, m1, v2, v3)
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
    printf("%s\n", kstr.c_str());

    int N = 256;
    int unit = 10;
    int batch = N / unit;
    int rest = N % unit;
    std::string macros;
    for (int i = 1; i < N; i++)
    {
        char buffer[1024];
        sprintf(buffer, "#define KCFG_FOR_EACH_%d(what, x, ...)\\", i + 1);
        macros.append(buffer).append("\n");
        macros.append("  what(x);\\").append("\n");
        sprintf(buffer, "  KCFG_FOR_EACH_%d(what,  __VA_ARGS__)", i);
        macros.append(buffer).append("\n");
    }
    macros.append("\n");
    macros.append("#define KCFG_NARG_(...)  KCFG_NARG_I_(__VA_ARGS__,KCFG_RSEQ_N())\n");
    macros.append("#define KCFG_NARG_I_(...) KCFG_ARG_N(__VA_ARGS__)\n");
    macros.append("#define KCFG_ARG_N( \\\n");
    for (int i = 0; i < batch; i++)
    {
        for (int j = 0; j < unit; j++)
        {
            char v[100];
            sprintf(v, "_%d,", i * unit + j + 1);
            macros.append(v);
        }
        macros.append("\\ \n");
    }
    for (int i = 0; i < rest - 1; i++)
    {
        char v[100];
        sprintf(v, "_%d,", batch * unit + i + 1);
        macros.append(v);
    }
    macros.append("N,...) N\n");
    macros.append("\n");
    macros.append("#define KCFG_RSEQ_N() \\\n");
    std::deque<int> cc;
    for (int i = 0; i < N - 1; i++)
    {
        cc.push_front(i);
    }
    while (!cc.empty())
    {
        int vv = cc.front();
        cc.pop_front();
        char v[100];
        if (vv == 0)
        {
            sprintf(v, "%d", vv);
        }
        else
        {
            sprintf(v, "%d,", vv);
        }
        macros.append(v);
        if (vv % 10 == 0 && vv != 0)
        {
            macros.append("\\\n");
        }
    }

    printf("%s\n", macros.c_str());

    return 0;
}

