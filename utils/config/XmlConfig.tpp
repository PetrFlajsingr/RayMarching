//
// Created by petr on 11/20/19.
//

template <typename T>
std::optional<T> findByName(tinyxml2::XMLElement *el, const KeysContainer &keys, unsigned int currentIndex) {
  if (auto der = dynamic_cast<const Xml::Tag *>(&(keys[currentIndex].get())); der != nullptr) {
    for (const auto &element : childrenByName(el, der->name)) {
      if (currentIndex == keys.size() - 1) {
        T result;
        from_xml(result, element);
        return result;
      } else if (auto res = findByName<T>(element, keys, currentIndex + 1); res.has_value()) {
        return res;
      }
    }
    return std::nullopt;
  } else {
    if (auto der = dynamic_cast<const Xml::Attribute *>(&(keys[currentIndex].get())); der != nullptr) {
      auto attr = el->Attribute(der->name.c_str(), der->value.c_str());
      if (attr != nullptr) {
        T result;
        from_xml(result, el);
        return result;
      }
      return std::nullopt;
    }
  }
  return std::nullopt;
}
template <typename T> bool setByName(tinyxml2::XMLDocument *doc, tinyxml2::XMLElement *el, const T &value,
                                     const KeysContainer &keys, unsigned int currentIndex) {
  if (auto der = dynamic_cast<const Xml::Tag *>(&(keys[currentIndex].get())); der != nullptr) {
    bool exists = false;
    for (const auto &element : childrenByName(el, der->name)) {
      exists = true;
      if (currentIndex == keys.size() - 1) {
        to_xml(value, element);
        return true;
      } else {
        if (!setByName<T>(doc, element, value, keys, currentIndex + 1)) {
          exists = false;
        } else {
          break;
        }
      }
    }
    if (!exists) {
      auto element = dynamic_cast<tinyxml2::XMLElement *>(el->LinkEndChild(doc->NewElement(der->name.c_str())));
      if (currentIndex == keys.size() - 1) {
        auto toSetElement = dynamic_cast<tinyxml2::XMLElement *>(element->LinkEndChild(doc->NewElement("")));
        to_xml(value, toSetElement);
        return true;
      }
      setByName<T>(doc, element, value, keys, currentIndex + 1);
    }
  } else {
    if (auto der = dynamic_cast<const Xml::Attribute *>(&(keys[currentIndex].get())); der != nullptr) {
      auto attr = el->Attribute(der->name.c_str());
      if (attr == nullptr || attr == der->value) {
        to_xml(value, el);
        return true;
      }
    }
  }
  return false;
}

template <typename T, typename... Keys>
std::optional<T> ConfigContainerTraits<XmlContainer>::find(XmlContainer &container, const Keys &... keys) {
  KeysContainer tmpKeys{keys...};
  auto element = container->RootElement();
  return findByName<T>(element, tmpKeys);
}
template <typename... Keys> bool ConfigContainerTraits<XmlContainer>::contains(XmlContainer &container, const Keys &... keys) {
  KeysContainer tmpKeys{keys...};
  auto element = container->RootElement();
  return Xml::findElement(element, tmpKeys) != nullptr;
}
template <typename T, typename... Keys>
void ConfigContainerTraits<XmlContainer>::set(XmlContainer &container, const T &value, const Keys &... keys) {
  KeysContainer tmpKeys{keys...};
  auto element = container->RootElement();
  if (tmpKeys.empty()) {
    auto newElement = dynamic_cast<tinyxml2::XMLElement *>(element->LinkEndChild(container->NewElement("")));
    to_xml(value, newElement);
    return;
  }
  setByName<T>(container.get(), element, value, tmpKeys);
}

XmlContainer ConfigLoader<XmlContainer>::load(std::string_view path) {
  auto result = std::make_unique<tinyxml2::XMLDocument>();
  result->LoadFile(std::string(path).c_str());
  return result;
}

void ConfigSaver<XmlContainer>::save(XmlContainer &config, std::string_view path) { config->SaveFile(std::string(path).c_str()); }
