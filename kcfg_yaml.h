/*
 *Copyright (c) 2022, yinqiwen <yinqiwen@gmail.com>
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
 *  * Neither the name of rimos nor the names of its contributors may be used
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
#pragma once

#include <stdint.h>
#include <stdio.h>

#include <boost/preprocessor/punctuation/remove_parens.hpp>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include "kcfg_common.h"

#include "yaml-cpp/emitter.h"
#include "yaml-cpp/node/convert.h"
#include "yaml-cpp/node/detail/impl.h"
#include "yaml-cpp/node/emit.h"
#include "yaml-cpp/node/impl.h"
#include "yaml-cpp/node/iterator.h"
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/parse.h"

namespace kcfg {

#define KCFG_YAML_DEFINE_FIELD_MAPPING(init)                                                      \
  const char* GetFieldName(const char* v) const {                                                 \
    static std::map<std::string, std::string> __field_mapping__ = {BOOST_PP_REMOVE_PARENS(init)}; \
    auto found = __field_mapping__.find(v);                                                       \
    if (found != __field_mapping__.end()) {                                                       \
      return found->second.c_str();                                                               \
    }                                                                                             \
    return v;                                                                                     \
  }

template <typename T>
static const char* GetFieldYamlName(T* t [[maybe_unused]], const char* v) {
  if constexpr (HasGetFieldName<T>::value) {
    return t->GetFieldName(v);
  } else {
    return v;
  }
}

template <typename T>
inline bool ParseYaml(const YAML::Node& doc, T& v) {
  if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float> || std::is_same_v<T, int8_t> ||
                std::is_same_v<T, uint8_t> || std::is_same_v<T, int16_t> || std::is_same_v<T, uint16_t> ||
                std::is_same_v<T, int32_t> || std::is_same_v<T, uint32_t> || std::is_same_v<T, int64_t> ||
                std::is_same_v<T, uint64_t> || std::is_same_v<T, bool> || std::is_same_v<T, char> ||
                std::is_same_v<T, std::string>) {
    if (!doc.IsScalar()) {
      return false;
    }
    try {
      v = doc.as<T>();
    } catch (...) {
      return false;
    }
    return true;
  } else {
    if (!doc.IsMap()) {
      return false;
    }
    return v.ParseFromYaml(doc);
  }
}

template <typename K>
inline bool ParseYaml(const YAML::Node& doc, std::vector<K>& v);
template <typename V>
inline bool ParseYaml(const YAML::Node& doc, std::map<std::string, V>& v);
template <typename V>
inline bool ParseYaml(const YAML::Node& doc, std::unordered_map<std::string, V>& v);
template <typename K>
inline bool ParseYaml(const YAML::Node& doc, std::deque<K>& v);

template <typename V>
inline bool ParseYaml(const YAML::Node& doc, std::map<std::string, V>& v) {
  if (!doc.IsMap()) {
    return false;
  }
  v.clear();
  for (auto it = doc.begin(); it != doc.end(); ++it) {
    try {
      std::string key = it->first.as<std::string>();  // <- key
      V tmp;
      if (ParseYaml(it->second, tmp)) {
        v.emplace(std::move(key), std::move(tmp));
      } else {
        return false;
      }
    } catch (...) {
      return false;
    }
  }
  return true;
}
template <typename V>
inline bool ParseYaml(const YAML::Node& doc, std::unordered_map<std::string, V>& v) {
  if (!doc.IsMap()) {
    return false;
  }
  v.clear();
  for (auto it = doc.begin(); it != doc.end(); ++it) {
    try {
      std::string key = it->first.as<std::string>();  // <- key
      V tmp;
      if (ParseYaml(it->second, tmp)) {
        v.emplace(std::move(key), std::move(tmp));
      } else {
        return false;
      }
    } catch (...) {
      return false;
    }
  }
  return true;
}
template <typename K>
inline bool ParseYaml(const YAML::Node& doc, std::vector<K>& v) {
  if (!doc.IsSequence()) {
    return false;
  }
  v.clear();
  for (auto it = doc.begin(); it != doc.end(); ++it) {
    const YAML::Node& node = *it;
    K tmp;
    if (ParseYaml(node, tmp)) {
      v.emplace_back(std::move(tmp));
    } else {
      return false;
    }
  }
  return true;
}
template <typename K>
inline bool ParseYaml(const YAML::Node& doc, std::deque<K>& v) {
  if (!doc.IsSequence()) {
    return false;
  }
  v.clear();
  for (auto it = doc.begin(); it != doc.end(); ++it) {
    const YAML::Node& node = *it;
    K tmp;
    if (ParseYaml(node, tmp)) {
      v.emplace_back(std::move(tmp));
    } else {
      return false;
    }
  }
  return true;
}
template <typename K>
inline bool ParseYaml(const YAML::Node& doc, std::list<K>& v) {
  if (!doc.IsSequence()) {
    return false;
  }
  v.clear();
  for (auto it = doc.begin(); it != doc.end(); ++it) {
    const YAML::Node& node = *it;
    K tmp;
    if (ParseYaml(node, tmp)) {
      v.emplace_back(std::move(tmp));
    } else {
      return false;
    }
  }
  return true;
}

template <typename K>
inline bool ParseYaml(const YAML::Node& doc, std::set<K>& v) {
  if (!doc.IsSequence()) {
    return false;
  }
  v.clear();
  for (auto it = doc.begin(); it != doc.end(); ++it) {
    const YAML::Node& node = *it;
    K tmp;
    if (ParseYaml(node, tmp)) {
      v.insert(std::move(tmp));
    } else {
      return false;
    }
  }
  return true;
}

template <typename T>
inline bool ParseFieldYaml(const YAML::Node& doc, const char* field, T& v) {
  if (!doc.IsMap()) {
    return false;
  }
  auto node = doc[field];
  if (!node.IsDefined() || node.IsNull()) {
    return false;
  }
  bool rc = ParseYaml(node, v);
  return rc;
}

template <typename T>
inline bool ParseFromYamlFile(const std::string& file, T& v) {
  YAML::Node config = YAML::LoadFile(file);
  if (!config.IsDefined() || config.IsNull()) {
    return false;
  }
  return ParseYaml(config, v);
}

template <typename T>
inline bool ParseFromYamlString(const std::string& content, T& v) {
  YAML::Node config = YAML::Load(content);
  if (!config.IsDefined() || config.IsNull()) {
    return false;
  }
  return ParseYaml(config, v);
}
}  // namespace kcfg

#define KCFG_YAML_STRINGIZE(arg) KCFG_YAML_STRINGIZE1(arg)
#define KCFG_YAML_STRINGIZE1(arg) KCFG_YAML_STRINGIZE2(arg)
#define KCFG_YAML_STRINGIZE2(arg) #arg
//
#define KCFG_YAML_CONCATENATE(arg1, arg2) KCFG_YAML_CONCATENATE1(arg1, arg2)
#define KCFG_YAML_CONCATENATE1(arg1, arg2) KCFG_YAML_CONCATENATE2(arg1, arg2)
#define KCFG_YAML_CONCATENATE2(arg1, arg2) arg1##arg2
//
#define KCFG_YAML_FOR_EACH_1(what, x, ...) what(x);

#define KCFG_YAML_FOR_EACH_2(what, x, ...) \
  what(x);                                 \
  KCFG_YAML_FOR_EACH_1(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_3(what, x, ...) \
  what(x);                                 \
  KCFG_YAML_FOR_EACH_2(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_4(what, x, ...) \
  what(x);                                 \
  KCFG_YAML_FOR_EACH_3(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_5(what, x, ...) \
  what(x);                                 \
  KCFG_YAML_FOR_EACH_4(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_6(what, x, ...) \
  what(x);                                 \
  KCFG_YAML_FOR_EACH_5(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_7(what, x, ...) \
  what(x);                                 \
  KCFG_YAML_FOR_EACH_6(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_8(what, x, ...) \
  what(x);                                 \
  KCFG_YAML_FOR_EACH_7(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_9(what, x, ...) \
  what(x);                                 \
  KCFG_YAML_FOR_EACH_8(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_10(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_9(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_11(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_10(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_12(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_11(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_13(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_12(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_14(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_13(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_15(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_14(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_16(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_15(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_17(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_16(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_18(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_17(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_19(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_18(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_20(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_19(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_21(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_20(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_22(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_21(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_23(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_22(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_24(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_23(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_25(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_24(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_26(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_25(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_27(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_26(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_28(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_27(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_29(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_28(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_30(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_29(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_31(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_30(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_32(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_31(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_33(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_32(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_34(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_33(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_35(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_34(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_36(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_35(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_37(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_36(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_38(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_37(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_39(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_38(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_40(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_39(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_41(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_40(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_42(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_41(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_43(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_42(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_44(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_43(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_45(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_44(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_46(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_45(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_47(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_46(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_48(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_47(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_49(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_48(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_50(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_49(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_51(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_50(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_52(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_51(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_53(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_52(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_54(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_53(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_55(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_54(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_56(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_55(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_57(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_56(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_58(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_57(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_59(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_58(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_60(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_59(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_61(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_60(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_62(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_61(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_63(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_62(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_64(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_63(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_65(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_64(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_66(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_65(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_67(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_66(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_68(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_67(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_69(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_68(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_70(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_69(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_71(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_70(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_72(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_71(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_73(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_72(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_74(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_73(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_75(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_74(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_76(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_75(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_77(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_76(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_78(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_77(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_79(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_78(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_80(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_79(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_81(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_80(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_82(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_81(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_83(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_82(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_84(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_83(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_85(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_84(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_86(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_85(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_87(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_86(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_88(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_87(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_89(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_88(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_90(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_89(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_91(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_90(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_92(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_91(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_93(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_92(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_94(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_93(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_95(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_94(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_96(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_95(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_97(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_96(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_98(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_97(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_99(what, x, ...) \
  what(x);                                  \
  KCFG_YAML_FOR_EACH_98(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_100(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_99(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_101(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_100(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_102(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_101(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_103(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_102(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_104(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_103(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_105(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_104(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_106(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_105(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_107(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_106(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_108(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_107(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_109(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_108(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_110(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_109(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_111(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_110(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_112(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_111(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_113(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_112(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_114(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_113(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_115(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_114(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_116(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_115(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_117(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_116(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_118(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_117(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_119(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_118(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_120(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_119(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_121(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_120(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_122(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_121(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_123(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_122(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_124(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_123(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_125(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_124(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_126(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_125(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_127(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_126(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_128(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_127(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_129(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_128(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_130(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_129(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_131(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_130(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_132(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_131(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_133(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_132(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_134(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_133(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_135(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_134(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_136(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_135(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_137(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_136(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_138(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_137(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_139(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_138(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_140(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_139(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_141(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_140(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_142(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_141(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_143(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_142(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_144(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_143(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_145(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_144(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_146(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_145(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_147(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_146(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_148(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_147(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_149(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_148(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_150(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_149(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_151(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_150(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_152(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_151(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_153(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_152(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_154(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_153(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_155(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_154(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_156(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_155(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_157(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_156(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_158(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_157(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_159(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_158(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_160(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_159(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_161(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_160(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_162(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_161(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_163(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_162(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_164(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_163(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_165(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_164(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_166(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_165(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_167(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_166(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_168(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_167(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_169(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_168(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_170(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_169(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_171(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_170(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_172(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_171(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_173(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_172(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_174(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_173(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_175(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_174(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_176(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_175(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_177(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_176(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_178(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_177(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_179(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_178(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_180(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_179(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_181(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_180(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_182(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_181(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_183(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_182(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_184(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_183(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_185(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_184(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_186(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_185(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_187(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_186(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_188(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_187(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_189(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_188(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_190(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_189(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_191(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_190(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_192(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_191(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_193(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_192(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_194(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_193(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_195(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_194(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_196(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_195(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_197(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_196(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_198(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_197(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_199(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_198(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_200(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_199(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_201(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_200(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_202(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_201(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_203(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_202(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_204(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_203(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_205(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_204(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_206(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_205(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_207(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_206(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_208(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_207(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_209(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_208(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_210(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_209(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_211(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_210(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_212(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_211(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_213(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_212(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_214(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_213(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_215(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_214(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_216(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_215(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_217(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_216(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_218(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_217(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_219(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_218(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_220(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_219(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_221(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_220(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_222(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_221(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_223(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_222(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_224(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_223(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_225(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_224(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_226(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_225(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_227(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_226(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_228(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_227(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_229(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_228(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_230(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_229(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_231(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_230(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_232(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_231(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_233(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_232(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_234(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_233(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_235(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_234(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_236(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_235(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_237(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_236(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_238(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_237(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_239(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_238(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_240(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_239(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_241(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_240(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_242(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_241(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_243(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_242(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_244(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_243(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_245(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_244(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_246(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_245(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_247(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_246(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_248(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_247(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_249(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_248(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_250(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_249(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_251(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_250(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_252(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_251(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_253(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_252(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_254(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_253(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_255(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_254(what, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH_256(what, x, ...) \
  what(x);                                   \
  KCFG_YAML_FOR_EACH_255(what, __VA_ARGS__)

#define KCFG_YAML_NARG_(...) KCFG_YAML_NARG_I_(__VA_ARGS__, KCFG_YAML_RSEQ_N())
#define KCFG_YAML_NARG_I_(...) KCFG_YAML_ARG_N(__VA_ARGS__)
#define KCFG_YAML_ARG_N(                                                                                               \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24,     \
    _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, \
    _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, \
    _71, _72, _73, _74, _75, _76, _77, _78, _79, _80, _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, _91, _92, _93, \
    _94, _95, _96, _97, _98, _99, _100, _101, _102, _103, _104, _105, _106, _107, _108, _109, _110, _111, _112, _113,  \
    _114, _115, _116, _117, _118, _119, _120, _121, _122, _123, _124, _125, _126, _127, _128, _129, _130, _131, _132,  \
    _133, _134, _135, _136, _137, _138, _139, _140, _141, _142, _143, _144, _145, _146, _147, _148, _149, _150, _151,  \
    _152, _153, _154, _155, _156, _157, _158, _159, _160, _161, _162, _163, _164, _165, _166, _167, _168, _169, _170,  \
    _171, _172, _173, _174, _175, _176, _177, _178, _179, _180, _181, _182, _183, _184, _185, _186, _187, _188, _189,  \
    _190, _191, _192, _193, _194, _195, _196, _197, _198, _199, _200, _201, _202, _203, _204, _205, _206, _207, _208,  \
    _209, _210, _211, _212, _213, _214, _215, _216, _217, _218, _219, _220, _221, _222, _223, _224, _225, _226, _227,  \
    _228, _229, _230, _231, _232, _233, _234, _235, _236, _237, _238, _239, _240, _241, _242, _243, _244, _245, _246,  \
    _247, _248, _249, _250, _251, _252, _253, _254, _255, N, ...)                                                      \
  N

#define KCFG_YAML_RSEQ_N()                                                                                             \
  255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235, 234, 233,   \
      232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211,    \
      210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189,    \
      188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167,    \
      166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145,    \
      144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123,    \
      122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101,    \
      100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, \
      72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45,  \
      44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17,  \
      16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define KCFG_YAML_FOR_EACH_(N, what, x, ...) KCFG_YAML_CONCATENATE(KCFG_YAML_FOR_EACH_, N)(what, x, __VA_ARGS__)
#define KCFG_YAML_FOR_EACH(what, ...) KCFG_YAML_FOR_EACH_(KCFG_YAML_NARG_(__VA_ARGS__), what, __VA_ARGS__)
#define KCFG_YAML_PARSE_YAML(field) kcfg::ParseFieldYaml(doc, kcfg::GetFieldYamlName(this, #field), field)

#define KCFG_YAML_DEFINE_FIELDS(...)                      \
  bool ParseFromYaml(const YAML::Node& doc) {             \
    KCFG_YAML_FOR_EACH(KCFG_YAML_PARSE_YAML, __VA_ARGS__) \
    return true;                                          \
  }
