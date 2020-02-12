//
// Created by Petr Flajsingr on 20/11/2019.
//

#ifndef UTILITIES_XMLCONFIG_H
#define UTILITIES_XMLCONFIG_H

#include "Config.h"
#include "tinyxml2/tinyxml2.h"
#include "various/XMLUtils.h"
#include <io/print.h>
#include <memory>
#include <vector>

namespace {
using XmlContainer = std::unique_ptr<tinyxml2::XMLDocument>;

using KeysContainer = std::vector<std::reference_wrapper<const Xml::Base>>;
} // namespace

template <typename T>
std::optional<T> findByName(tinyxml2::XMLElement *el, const KeysContainer &keys, unsigned int currentIndex = 0);

template <typename T> bool setByName(tinyxml2::XMLDocument *doc, tinyxml2::XMLElement *el, const T &value,
                                     const KeysContainer &keys, unsigned int currentIndex = 0);

template <> struct ConfigContainerTraits<XmlContainer> {
  using KeysContainer = std::vector<std::reference_wrapper<const Xml::Base>>;

  template <typename T, typename... Keys> static std::optional<T> find(XmlContainer &container, const Keys &... keys);

  template <typename... Keys> static bool contains(XmlContainer &container, const Keys &... keys);

  template <typename T, typename... Keys> static void set(XmlContainer &container, const T &value, const Keys &... keys);
};

template <> struct ConfigLoader<XmlContainer> { XmlContainer load(std::string_view path); };

template <> struct ConfigSaver<XmlContainer> { void save(XmlContainer &config, std::string_view path); };

template <bool ReadOnly> using XmlConfig = Config<XmlContainer, ReadOnly>;

#include "XmlConfig.tpp"

#endif // UTILITIES_XMLCONFIG_H
