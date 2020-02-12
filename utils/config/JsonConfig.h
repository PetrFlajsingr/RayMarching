//
// Created by Petr on 18.11.2019.
//

#ifndef UTILITIES_JSONCONFIG_H
#define UTILITIES_JSONCONFIG_H

#include "Config.h"
#include "io/print.h"
#include "types/Range.h"
#include <fstream>
#include <nlohmann/json.hpp>

namespace {
using namespace MakeRange;

template <typename... Keys> std::optional<nlohmann::json::iterator> findJsonNode(nlohmann::json &container, const Keys &... keys);

template <typename T, typename... Keys> void setJsonNode(nlohmann::json &container, const T &value, const Keys &... keys);
} // namespace

template <> struct ConfigContainerTraits<nlohmann::json> {
  template <typename T, typename... Keys> static std::optional<T> find(nlohmann::json &container, const Keys &... keys);

  template <typename... Keys> static bool contains(nlohmann::json &container, const Keys &... keys);

  template <typename T, typename... Keys> static void set(nlohmann::json &container, const T &value, const Keys &... keys);
};

template <> struct ConfigLoader<nlohmann::json> { nlohmann::json load(std::string_view path); };

template <> struct ConfigSaver<nlohmann::json> { void save(nlohmann::json &config, std::string_view path); };

template <bool ReadOnly> using JsonConfig = Config<nlohmann::json, ReadOnly>;

#include "JsonConfig.tpp"

#endif // UTILITIES_JSONCONFIG_H
