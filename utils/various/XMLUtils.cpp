//
// Created by Petr Flajsingr on 20/11/2019.
//

#include "XMLUtils.h"

Xml::Tag::Tag(std::string name) : Base(), name(name) {}
Xml::Attribute::Attribute(std::string name, std::string value) : Base(), name(name), value(value) {}
Xml::Index::Index(int index) : index(index) {}
NamedSiblingIterator::NamedSiblingIterator(tinyxml2::XMLElement *element) : current(element), valid(element != nullptr) {}
NamedSiblingIterator &NamedSiblingIterator::operator++() {
  current = current->NextSiblingElement(current->Name());
  if (current == nullptr) {
    valid = false;
  }
  return *this;
}
NamedSiblingIterator NamedSiblingIterator::operator++(int) {
  NamedSiblingIterator next{current};
  current = current->NextSiblingElement(current->Name());
  if (current == nullptr) {
    valid = false;
  }
  return next;
}
tinyxml2::XMLElement *NamedSiblingIterator::operator*() { return current; }
bool NamedSiblingIterator::operator==(const NamedSiblingIterator &rhs) const {
  return current == rhs.current && !valid && !rhs.valid;
}
bool NamedSiblingIterator::operator!=(const NamedSiblingIterator &rhs) const { return !(rhs == *this); }
NamedSiblingIterator::operator bool() { return valid; }
NamedSiblingIterator NamedSiblingIterator::Invalid() {
  NamedSiblingIterator result{nullptr};
  result.valid = false;
  return result;
}
NamedSiblingIterator NamedSiblingIteratorInterface::begin() { return current; }
NamedSiblingIterator NamedSiblingIteratorInterface::end() { return nullptr; }
NamedSiblingIteratorInterface::NamedSiblingIteratorInterface(tinyxml2::XMLElement *element) : current(element) {}
NamedSiblingIteratorInterface childrenByName(tinyxml2::XMLElement *element, const std::string &name) {
  return NamedSiblingIteratorInterface{element->FirstChildElement(name.c_str())};
}
