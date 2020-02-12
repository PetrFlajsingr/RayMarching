//
// Created by Petr Flajsingr on 20/11/2019.
//

#ifndef UTILITIES_XMLUTILS_H
#define UTILITIES_XMLUTILS_H
#include <meta/string.h>
#include <string>
#include <tinyxml2/tinyxml2.h>

template <typename T> void to_xml(const T &value, tinyxml2::XMLElement *elem) {
  if constexpr (is_string_v<T>) {
    elem->SetValue("string");
    elem->SetText(value.c_str());
  } else if constexpr (std::is_fundamental_v<T>) {
    elem->SetValue("number");
    elem->SetValue(value);
  }
}

template <typename T> void from_xml(T &value, tinyxml2::XMLElement *elem) {
  static_assert(is_string_v<T> || std::is_fundamental_v<T>);
  if constexpr (is_string_v<T>) {
    value = elem->GetText();
  } else if constexpr (std::is_fundamental_v<T>) {
    if constexpr (std::is_same_v<T, int>) {
      elem->QueryIntText(&value);
    }
    if constexpr (std::is_same_v<T, unsigned>) {
      elem->QueryUnsignedText(&value);
    }
    if constexpr (std::is_same_v<T, int64_t>) {
      elem->QueryInt64Text(&value);
    }
    if constexpr (std::is_same_v<T, uint64_t>) {
      elem->QueryUnsigned64Text(&value);
    }
    if constexpr (std::is_same_v<T, bool>) {
      elem->QueryBoolText(&value);
    }
    if constexpr (std::is_same_v<T, double>) {
      elem->QueryDoubleText(&value);
    }
    if constexpr (std::is_same_v<T, float>) {
      elem->QueryFloatText(&value);
    }
  }
}

namespace Xml {
struct Base {
  virtual ~Base() = default;
};
struct Tag : public Base {
  Tag(std::string name);
  std::string name;
};
struct Attribute : public Base {
  Attribute(std::string name, std::string value);
  std::string name;
  std::string value;
};
struct Index : public Base {
  Index(int index);
  int index;
};
} // namespace Xml

class NamedSiblingIterator {
  tinyxml2::XMLElement *current;
  bool valid = true;

public:
  NamedSiblingIterator(tinyxml2::XMLElement *element);
  static NamedSiblingIterator Invalid();

  NamedSiblingIterator &operator++();

  NamedSiblingIterator operator++(int);

  tinyxml2::XMLElement *operator*();

  bool operator==(const NamedSiblingIterator &rhs) const;
  bool operator!=(const NamedSiblingIterator &rhs) const;

  operator bool();
};

class NamedSiblingIteratorInterface {
  tinyxml2::XMLElement *current;

public:
  NamedSiblingIteratorInterface(tinyxml2::XMLElement *element);
  NamedSiblingIterator begin();
  NamedSiblingIterator end();
};

NamedSiblingIteratorInterface childrenByName(tinyxml2::XMLElement *element, const std::string &name);

namespace Xml {
using KeysContainer = std::vector<std::reference_wrapper<const Xml::Base>>;
template <typename... Args>
tinyxml2::XMLElement *findElement(tinyxml2::XMLElement *el, const KeysContainer &keys, unsigned int currentIndex = 0) {
  if (auto der = dynamic_cast<const Xml::Tag *>(&(keys[currentIndex].get())); der != nullptr) {
    for (const auto &element : childrenByName(el, der->name)) {
      if (currentIndex == keys.size() - 1) {
        return element;
      } else if (auto res = findElement(element, keys, currentIndex + 1); res != nullptr) {
        return res;
      }
    }
    return nullptr;
  } else {
    if (auto der = dynamic_cast<const Xml::Attribute *>(&(keys[currentIndex].get())); der != nullptr) {
      auto attr = el->Attribute(der->name.c_str(), der->value.c_str());
      if (attr != nullptr) {
        return el;
      }
      return nullptr;
    }
  }
  return nullptr;
}
} // namespace Xml

#endif // UTILITIES_XMLUTILS_H
