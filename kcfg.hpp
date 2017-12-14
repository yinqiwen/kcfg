/*
 * kcfg.hpp
 *
 *  Created on: 2017Äê12ÔÂ14ÈÕ
 *      Author: qiyingwang
 */

#ifndef KCFG_HPP_
#define KCFG_HPP_
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <string>
#include <map>
#include <vector>
#include <list>
#include <deque>
#include <stdio.h>

namespace kcfg
{
    const rapidjson::Value* getJsonFiledValue(const rapidjson::Value& json, const char* name)
    {
        if (name == "")
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
    bool Parse(const rapidjson::Value& json, const char* name, T& v)
    {
        const rapidjson::Value* val = getJsonFiledValue(json, name);
        if (NULL == val)
        {
            return false;
        }
        if (val->IsObject())
        {
            return false;
        }
        return v.PraseFromJson(json);
    }

    template<>
    bool Parse(const rapidjson::Value& json, const char* name, double& v)
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
    bool Parse(const rapidjson::Value& json, const char* name, float& v)
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
    bool Parse(const rapidjson::Value& json, const char* name, std::string& v)
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
    bool Parse(const rapidjson::Value& json, const char* name, uint64_t& v)
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
    bool Parse(const rapidjson::Value& json, const char* name, int64_t& v)
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
    bool Parse(const rapidjson::Value& json, const char* name, int32_t& v)
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
    bool Parse(const rapidjson::Value& json, const char* name, uint32_t& v)
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
    bool Parse(const rapidjson::Value& json, const char* name, int16_t& v)
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
    bool Parse(const rapidjson::Value& json, const char* name, uint16_t& v)
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
    bool Parse(const rapidjson::Value& json, const char* name, int8_t& v)
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
    bool Parse(const rapidjson::Value& json, const char* name, uint8_t& v)
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

    template<typename V>
    bool Parse(const rapidjson::Value& json, const char* name, std::map<std::string, V>& v)
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
    bool Parse(const rapidjson::Value& json, const char* name, std::vector<K>& v)
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
    bool Parse(const rapidjson::Value& json, const char* name, std::deque<K>& v)
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
    bool Parse(const rapidjson::Value& json, const char* name, std::list<K>& v)
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

    void addJsonMember(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name, rapidjson::Value& json_value)
    {
        if (name == "")
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
    void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name, const T& v)
    {
        rapidjson::Value json_value(rapidjson::kObjectType);
        v.WriteToJson(json_value);
        addJsonMember(json, allocator, name, json_value);
    }

    template<>
    void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const double& v)
    {
        rapidjson::Value json_value(rapidjson::kNumberType);
        json_value.SetDouble(v);
        addJsonMember(json, allocator, name, json_value);
    }

    template<>
    void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name, const float& v)
    {
        rapidjson::Value json_value(rapidjson::kNumberType);
        json_value.SetDouble(v);
        addJsonMember(json, allocator, name, json_value);
    }
    template<>
    void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const std::string& v)
    {
        rapidjson::Value json_value(v.c_str(), v.size());
        addJsonMember(json, allocator, name, json_value);
    }

    template<>
    void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const uint64_t& v)
    {
        rapidjson::Value json_value(rapidjson::kNumberType);
        json_value.SetUint64(v);
        addJsonMember(json, allocator, name, json_value);
    }

    template<>
    void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const int64_t& v)
    {
        rapidjson::Value json_value(rapidjson::kNumberType);
        json_value.SetInt64(v);
        addJsonMember(json, allocator, name, json_value);
    }

    template<>
    void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const int32_t& v)
    {
        int64_t vv = v;
        Serialize(json, allocator, name, vv);
    }
    template<>
    void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const uint32_t& v)
    {
        uint64_t vv = v;
        Serialize(json, allocator, name, vv);
    }
    template<>
    void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const int16_t& v)
    {
        int64_t vv = v;
        Serialize(json, allocator, name, vv);
    }
    template<>
    void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const uint16_t& v)
    {
        uint64_t vv = v;
        Serialize(json, allocator, name, vv);
    }
    template<>
    void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const int8_t& v)
    {
        int64_t vv = v;
        Serialize(json, allocator, name, vv);
    }
    template<>
    void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const uint8_t& v)
    {
        uint64_t vv = v;
        Serialize(json, allocator, name, vv);
    }

    template<typename V>
    void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
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
    void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
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
    void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
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
    void Serialize(rapidjson::Value& json, rapidjson::Value::AllocatorType& allocator, const char* name,
            const std::list<K>& v)
    {
        rapidjson::Value json_value(rapidjson::kArrayType);
        for (size_t i = 0; i < v.size(); i++)
        {
            const K& vv = v[i];
            Serialize(json_value, allocator, "", vv);
        }
        addJsonMember(json, allocator, name, json_value);
    }
}

#define STRINGIZE(arg)  STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg
//
#define CONCATENATE(arg1, arg2)   CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2)  CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2)  arg1##arg2
//
#define FOR_EACH_1(what, x, ...) what(x);

#define FOR_EACH_2(what, x, ...)\
  what(x);\
  FOR_EACH_1(what,  __VA_ARGS__)
#define FOR_EACH_3(what, x, ...)\
  what(x);\
  FOR_EACH_2(what, __VA_ARGS__)
#define FOR_EACH_4(what, x, ...)\
  what(x);\
  FOR_EACH_3(what,  __VA_ARGS__)
#define FOR_EACH_5(what, x, ...)\
  what(x);\
 FOR_EACH_4(what,  __VA_ARGS__)
#define FOR_EACH_6(what, x, ...)\
  what(x);\
  FOR_EACH_5(what,  __VA_ARGS__)
#define FOR_EACH_7(what, x, ...)\
  what(x);\
  FOR_EACH_6(what,  __VA_ARGS__)
#define FOR_EACH_8(what, x, ...)\
  what(x);\
  FOR_EACH_7(what,  __VA_ARGS__)
#define FOR_EACH_9(what, x, ...)\
  what(x);\
  FOR_EACH_8(what,  __VA_ARGS__)
#define FOR_EACH_10(what, x, ...)\
  what(x);\
  FOR_EACH_9(what,  __VA_ARGS__)

#define __NARG__(...)  __NARG_I_(__VA_ARGS__,__RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N( \
      _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
     _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
     _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
     _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
     _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
     _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
     _61,_62,_63,N,...) N
#define __RSEQ_N() \
     63,62,61,60,                   \
     59,58,57,56,55,54,53,52,51,50, \
     49,48,47,46,45,44,43,42,41,40, \
     39,38,37,36,35,34,33,32,31,30, \
     29,28,27,26,25,24,23,22,21,20, \
     19,18,17,16,15,14,13,12,11,10, \
     9,8,7,6,5,4,3,2,1,0

#define FOR_EACH_(N, what, x, ...) CONCATENATE(FOR_EACH_, N)(what, x, __VA_ARGS__)
#define FOR_EACH(what, ...) FOR_EACH_(__NARG__(__VA_ARGS__), what, __VA_ARGS__)
#define PARSE_JSON(field)   kcfg::Parse(json, #field, field)
#define SERIALIZE_JSON(field)   kcfg::Serialize(json, allocator, #field, field)
#define PRN_FIELD(field)  printf("%s\n", #field)
#define KCFG_DEFINE_FIELDS(...) \
    bool PraseFromJson(const rapidjson::Value& json)  \
    { \
         FOR_EACH(PARSE_JSON, __VA_ARGS__)\
         return true; \
    } \
    void WriteToJson(rapidjson::Value& json) const \
    {  \
          rapidjson::Value::AllocatorType allocator;\
          FOR_EACH(SERIALIZE_JSON, __VA_ARGS__)  \
    } \

#endif /* KCFG_HPP_ */
