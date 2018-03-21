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

#ifndef KCFG_HPP_
#define KCFG_HPP_
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <string>
#include <map>
#include <vector>
#include <list>
#include <deque>
#include <stdio.h>
#include <stdint.h>

namespace kcfg
{
    inline const rapidjson::Value* getJsonFiledValue(const rapidjson::Value& json, const char* name)
    {
        if (!name[0])
        {
            return &json;
        }
        rapidjson::Value::ConstMemberIterator found = json.FindMember(name);
        if (found == json.MemberEnd())
        {
            return NULL;
        }
        return &(found->value);
    }

    template<typename T>
    inline bool Parse(const rapidjson::Value& json, const char* name, T& v)
    {
        const rapidjson::Value* val = getJsonFiledValue(json, name);
        if (NULL == val)
        {
            return false;
        }
        if (!val->IsObject())
        {
            return false;
        }
        return v.PraseFromJson(*val);
    }

    template<>
    inline bool Parse(const rapidjson::Value& json, const char* name, double& v)
    {
        const rapidjson::Value* val = getJsonFiledValue(json, name);
        if (NULL == val)
        {
            return false;
        }
        if (!val->IsDouble())
        {
            return false;
        }
        v = val->GetDouble();
        return true;
    }

    template<>
    inline bool Parse(const rapidjson::Value& json, const char* name, float& v)
    {
        const rapidjson::Value* val = getJsonFiledValue(json, name);
        if (NULL == val)
        {
            return false;
        }
        if (!val->IsFloat())
        {
            return false;
        }
        v = val->GetFloat();
        return true;
    }
    template<>
    inline bool Parse(const rapidjson::Value& json, const char* name, std::string& v)
    {
        const rapidjson::Value* val = getJsonFiledValue(json, name);
        if (NULL == val)
        {
            return false;
        }
        if (!val->IsString())
        {
            return false;
        }
        v.assign(val->GetString(), val->GetStringLength());
        return true;
    }

    template<>
    inline bool Parse(const rapidjson::Value& json, const char* name, uint64_t& v)
    {
        const rapidjson::Value* val = getJsonFiledValue(json, name);
        if (NULL == val)
        {
            return false;
        }
        if (!val->IsUint64() && !val->IsUint())
        {
            return false;
        }
        if (val->IsUint64())
        {
            v = val->GetUint64();
        }
        else
        {
            v = val->GetUint();
        }
        return true;
    }

    template<>
    inline bool Parse(const rapidjson::Value& json, const char* name, int64_t& v)
    {
        const rapidjson::Value* val = getJsonFiledValue(json, name);
        if (NULL == val)
        {
            return false;
        }
        if (!val->IsInt64() && !val->IsInt())
        {
            return false;
        }
        if (val->IsInt64())
        {
            v = val->GetInt64();
        }
        else
        {
            v = val->GetInt();
        }
        return true;
    }

    template<>
    inline bool Parse(const rapidjson::Value& json, const char* name, int32_t& v)
    {
        int64_t vv;
        if (!Parse(json, name, vv))
        {
            return false;
        }
        if (vv > INT32_MAX || vv < INT32_MIN)
        {
            return false;
        }
        v = (int32_t) vv;
        return true;
    }
    template<>
    inline bool Parse(const rapidjson::Value& json, const char* name, uint32_t& v)
    {
        uint64_t vv;
        if (!Parse(json, name, vv))
        {
            return false;
        }
        if (vv > UINT32_MAX)
        {
            return false;
        }
        v = (uint32_t) vv;
        return true;
    }
    template<>
    inline bool Parse(const rapidjson::Value& json, const char* name, int16_t& v)
    {
        int64_t vv;
        if (!Parse(json, name, vv))
        {
            return false;
        }
        if (vv > INT16_MAX || vv < INT16_MIN)
        {
            return false;
        }
        v = (int16_t) vv;
        return true;
    }
    template<>
    inline bool Parse(const rapidjson::Value& json, const char* name, uint16_t& v)
    {
        uint64_t vv;
        if (!Parse(json, name, vv))
        {
            return false;
        }
        if (vv > UINT16_MAX)
        {
            return false;
        }
        v = (uint16_t) vv;
        return true;
    }
    template<>
    inline bool Parse(const rapidjson::Value& json, const char* name, int8_t& v)
    {
        int64_t vv;
        if (!Parse(json, name, vv))
        {
            return false;
        }
        if (vv > INT8_MAX || vv < INT8_MIN)
        {
            return false;
        }
        v = (int8_t) vv;
        return true;
    }
    template<>
    inline bool Parse(const rapidjson::Value& json, const char* name, uint8_t& v)
    {
        int64_t vv;
        if (!Parse(json, name, vv))
        {
            return false;
        }
        if (vv > UINT8_MAX)
        {
            return false;
        }
        v = (uint8_t) vv;
        return true;
    }

    template<>
    inline bool Parse(const rapidjson::Value& json, const char* name, bool& v)
    {
        const rapidjson::Value* val = getJsonFiledValue(json, name);
        if (NULL == val)
        {
            return false;
        }
        if (!val->IsBool())
        {
            return false;
        }
        v = val->GetBool();
        return true;
    }

    template<typename V>
    inline bool Parse(const rapidjson::Value& json, const char* name, std::map<std::string, V>& v)
    {
        const rapidjson::Value* val = getJsonFiledValue(json, name);
        if (NULL == val)
        {
            return false;
        }
        if (!val->IsObject())
        {
            return false;
        }
        rapidjson::Value::ConstMemberIterator it = val->MemberBegin();
        while (it != val->MemberEnd())
        {
            V tmp;
            std::string fieldName = it->name.GetString();
            if (Parse(it->value, "", tmp))
            {
                v[fieldName] = tmp;
            }
            else
            {
                v.clear();
                return false;
            }
            it++;
        }
        return true;
    }
    template<typename K>
    inline bool Parse(const rapidjson::Value& json, const char* name, std::vector<K>& v)
    {
        const rapidjson::Value* val = getJsonFiledValue(json, name);
        if (NULL == val)
        {
            return false;
        }
        if (!val->IsArray())
        {
            return false;
        }
        for (rapidjson::Value::ConstValueIterator ait = val->Begin(); ait != val->End(); ++ait)
        {
            K tmp;
            if (Parse(*ait, "", tmp))
            {
                v.push_back(tmp);
            }
            else
            {
                v.clear();
                return false;
            }
        }
        return true;
    }
    template<typename K>
    inline bool Parse(const rapidjson::Value& json, const char* name, std::deque<K>& v)
    {
        const rapidjson::Value* val = getJsonFiledValue(json, name);
        if (NULL == val)
        {
            return false;
        }
        if (!val->IsArray())
        {
            return false;
        }
        for (rapidjson::Value::ConstValueIterator ait = val->Begin(); ait != val->End(); ++ait)
        {
            K tmp;
            if (Parse(*ait, "", tmp))
            {
                v.push_back(tmp);
            }
            else
            {
                v.clear();
                return false;
            }
        }
        return true;
    }
    template<typename K>
    inline bool Parse(const rapidjson::Value& json, const char* name, std::list<K>& v)
    {
        const rapidjson::Value* val = getJsonFiledValue(json, name);
        if (NULL == val)
        {
            return false;
        }
        if (!val->IsArray())
        {
            return false;
        }
        for (rapidjson::Value::ConstValueIterator ait = val->Begin(); ait != val->End(); ++ait)
        {
            K tmp;
            if (Parse(*ait, "", tmp))
            {
                v.push_back(tmp);
            }
            else
            {
                v.clear();
                return false;
            }
        }
        return true;
    }

    inline void addJsonMember(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            rapidjson::Value& json_value)
    {
        if (!name[0])
        {
            json.PushBack(json_value, allocator);
        }
        else
        {
            rapidjson::Value field_name(name, strlen(name));
            json.AddMember(field_name, json_value, allocator);
        }
    }

    template<typename T>
    inline void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const T& v)
    {
        rapidjson::Value json_value(rapidjson::kObjectType);
        v.WriteToJson(json_value, allocator);
        addJsonMember(json, allocator, name, json_value);
    }

    template<>
    inline void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const double& v)
    {
        rapidjson::Value json_value(rapidjson::kNumberType);
        json_value.SetDouble(v);
        addJsonMember(json, allocator, name, json_value);
    }

    template<>
    inline void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const float& v)
    {
        rapidjson::Value json_value(rapidjson::kNumberType);
        json_value.SetDouble(v);
        addJsonMember(json, allocator, name, json_value);
    }
    template<>
    inline void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const std::string& v)
    {
        rapidjson::Value json_value(v.c_str(), v.size(), allocator);
        addJsonMember(json, allocator, name, json_value);
    }
    template<>
    inline void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const bool& v)
    {
        rapidjson::Value json_value(rapidjson::kNumberType);
        json_value.SetBool(v);
        addJsonMember(json, allocator, name, json_value);
    }
    template<>
    inline void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const uint64_t& v)
    {
        rapidjson::Value json_value(rapidjson::kNumberType);
        json_value.SetUint64(v);
        addJsonMember(json, allocator, name, json_value);
    }

    template<>
    inline void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const int64_t& v)
    {
        rapidjson::Value json_value(rapidjson::kNumberType);
        json_value.SetInt64(v);
        addJsonMember(json, allocator, name, json_value);
    }

    template<>
    inline void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const int32_t& v)
    {
        int64_t vv = v;
        Serialize(json, allocator, name, vv);
    }
    template<>
    inline void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const uint32_t& v)
    {
        uint64_t vv = v;
        Serialize(json, allocator, name, vv);
    }
    template<>
    inline void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const int16_t& v)
    {
        int64_t vv = v;
        Serialize(json, allocator, name, vv);
    }
    template<>
    inline void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const uint16_t& v)
    {
        uint64_t vv = v;
        Serialize(json, allocator, name, vv);
    }
    template<>
    inline void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const int8_t& v)
    {
        int64_t vv = v;
        Serialize(json, allocator, name, vv);
    }
    template<>
    inline void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const uint8_t& v)
    {
        uint64_t vv = v;
        Serialize(json, allocator, name, vv);
    }

    template<typename V>
    inline void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const std::map<std::string, V>& v)
    {
        rapidjson::Value json_value(rapidjson::kObjectType);
        typename std::map<std::string, V>::const_iterator it = v.begin();
        while (it != v.end())
        {
            const V& vv = it->second;
            Serialize(json_value, allocator, it->first.c_str(), vv);
            it++;
        }
        addJsonMember(json, allocator, name, json_value);
    }
    template<typename K>
    inline void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const std::deque<K>& v)
    {
        rapidjson::Value json_value(rapidjson::kArrayType);
        for (size_t i = 0; i < v.size(); i++)
        {
            const K& vv = v[i];
            Serialize(json_value, allocator, "", vv);
        }
        addJsonMember(json, allocator, name, json_value);
    }
    template<typename K>
    inline void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const std::vector<K>& v)
    {
        rapidjson::Value json_value(rapidjson::kArrayType);
        for (size_t i = 0; i < v.size(); i++)
        {
            const K& vv = v[i];
            Serialize(json_value, allocator, "", vv);
        }
        addJsonMember(json, allocator, name, json_value);
    }
    template<typename K>
    inline void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const std::list<K>& v)
    {
        rapidjson::Value json_value(rapidjson::kArrayType);
        typename std::list<K>::const_iterator cit = v.begin();
        while (cit != v.end())
        {
            const K& vv = *cit;
            Serialize(json_value, allocator, "", vv);
            cit++;
        }
        addJsonMember(json, allocator, name, json_value);
    }
    template<typename T>
    inline bool ParseFromJsonString(const std::string& content, T& v)
    {
        rapidjson::Document d;
        d.Parse<0>(content.c_str());
        if (d.HasParseError())
        {
            return false;
        }
        return Parse(d, "", v);
    }

    template<typename T>
    inline bool ParseFromJsonFile(const std::string& file, T& v)
    {
        FILE *fp;
        if ((fp = fopen(file.c_str(), "rb")) == NULL)
        {
            return false;
        }

        fseek(fp, 0, SEEK_END);
        long int fsize = ftell(fp);
        if (fsize >= 100 * 1024 * 1024)
        {
            //too large for a config file
            fclose(fp);
            return false;
        }
        rewind(fp);
        char* buffer = (char*) malloc(fsize);
        if (NULL == buffer)
        {
            fclose(fp);
            return false;
        }
        std::string content;
        size_t len = fread(buffer, 1, fsize, fp);
        content.assign(buffer, len);
        free(buffer);
        fclose(fp);
        return ParseFromJsonString(content, v);
    }

    template<typename T>
    inline int WriteToJsonString(const T& v, std::string& content)
    {
        rapidjson::Value::AllocatorType allocator;
        rapidjson::Value doc(rapidjson::kObjectType);
        v.WriteToJson(doc, allocator);
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        content.assign(buffer.GetString(), buffer.GetSize());
        return 0;
    }

    template<typename T>
    inline int WriteToJsonFile(const T& v, const std::string& file)
    {
        std::string content;
        int ret = WriteToJsonString(v, content);
        if (0 != ret)
        {
            return ret;
        }
        FILE *fp;
        if ((fp = fopen(file.c_str(), "wb")) == NULL)
        {
            return -1;
        }
        fwrite(content.c_str(), content.size(), 1, fp);
        fclose(fp);
        return 0;
    }
}

#define KCFG_STRINGIZE(arg)  KCFG_STRINGIZE1(arg)
#define KCFG_STRINGIZE1(arg) KCFG_STRINGIZE2(arg)
#define KCFG_STRINGIZE2(arg) #arg
//
#define KCFG_CONCATENATE(arg1, arg2)   KCFG_CONCATENATE1(arg1, arg2)
#define KCFG_CONCATENATE1(arg1, arg2)  KCFG_CONCATENATE2(arg1, arg2)
#define KCFG_CONCATENATE2(arg1, arg2)  arg1##arg2
//
#define KCFG_FOR_EACH_1(what, x, ...) what(x);

#define KCFG_FOR_EACH_2(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_1(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_3(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_2(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_4(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_3(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_5(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_4(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_6(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_5(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_7(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_6(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_8(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_7(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_9(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_8(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_10(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_9(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_11(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_10(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_12(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_11(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_13(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_12(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_14(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_13(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_15(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_14(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_16(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_15(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_17(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_16(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_18(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_17(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_19(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_18(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_20(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_19(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_21(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_20(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_22(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_21(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_23(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_22(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_24(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_23(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_25(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_24(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_26(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_25(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_27(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_26(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_28(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_27(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_29(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_28(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_30(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_29(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_31(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_30(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_32(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_31(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_33(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_32(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_34(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_33(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_35(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_34(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_36(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_35(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_37(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_36(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_38(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_37(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_39(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_38(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_40(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_39(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_41(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_40(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_42(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_41(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_43(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_42(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_44(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_43(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_45(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_44(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_46(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_45(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_47(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_46(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_48(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_47(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_49(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_48(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_50(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_49(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_51(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_50(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_52(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_51(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_53(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_52(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_54(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_53(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_55(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_54(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_56(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_55(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_57(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_56(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_58(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_57(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_59(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_58(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_60(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_59(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_61(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_60(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_62(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_61(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_63(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_62(what,  __VA_ARGS__)
#define KCFG_FOR_EACH_64(what, x, ...)\
  what(x);\
  KCFG_FOR_EACH_63(what,  __VA_ARGS__)

#define KCFG_NARG_(...)  KCFG_NARG_I_(__VA_ARGS__,KCFG_RSEQ_N())
#define KCFG_NARG_I_(...) KCFG_ARG_N(__VA_ARGS__)
#define KCFG_ARG_N( \
      _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
     _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
     _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
     _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
     _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
     _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
     _61,_62,_63,N,...) N
#define KCFG_RSEQ_N() \
     63,62,61,60,                   \
     59,58,57,56,55,54,53,52,51,50, \
     49,48,47,46,45,44,43,42,41,40, \
     39,38,37,36,35,34,33,32,31,30, \
     29,28,27,26,25,24,23,22,21,20, \
     19,18,17,16,15,14,13,12,11,10, \
     9,8,7,6,5,4,3,2,1,0

#define KCFG_FOR_EACH_(N, what, x, ...) KCFG_CONCATENATE(KCFG_FOR_EACH_, N)(what, x, __VA_ARGS__)
#define KCFG_FOR_EACH(what, ...) KCFG_FOR_EACH_(KCFG_NARG_(__VA_ARGS__), what, __VA_ARGS__)
#define KCFG_PARSE_JSON(field)   kcfg::Parse(json, #field, field)
#define KCFG_SERIALIZE_JSON(field)   kcfg::Serialize(json, allocator, #field, field)
//#define PRN_FIELD(field)  printf("%s\n", #field)
#define KCFG_DEFINE_FIELDS(...) \
    bool PraseFromJson(const rapidjson::Value& json)  \
    { \
         KCFG_FOR_EACH(KCFG_PARSE_JSON, __VA_ARGS__)\
         return true; \
    } \
    void WriteToJson(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator) const \
    {  \
          KCFG_FOR_EACH(KCFG_SERIALIZE_JSON, __VA_ARGS__)  \
    } \

#endif /* KCFG_HPP_ */
