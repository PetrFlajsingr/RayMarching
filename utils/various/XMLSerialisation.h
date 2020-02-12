//
// Created by petr on 11/20/19.
//

#ifndef UTILITIES_XMLSERIALISATION_H
#define UTILITIES_XMLSERIALISATION_H

#include "XMLUtils.h"

template <typename T> void to_xml(const T &value, tinyxml2::XMLElement *elem);

template <typename T> void from_xml(T &value, tinyxml2::XMLElement *elem);

#endif // UTILITIES_XMLSERIALISATION_H
