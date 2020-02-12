//
// Created by Petr on 18.11.2019.
//

#ifndef UTILITIES_STRING_H
#define UTILITIES_STRING_H

#include "various/StringUtils.h"
#include <string>

template <typename T> struct is_string : std::false_type {};

template <class CharT, class Traits, class Alloc> struct is_string<std::basic_string<CharT, Traits, Alloc>> : std::true_type {};

template <> struct is_string<const char *> : std::true_type {};
template <int N> struct is_string<const char[N]> : std::true_type {};
template <int N> struct is_string<const char (&)[N]> : std::true_type {};

template <class CharT, class Traits, class Alloc> struct is_string<StringDecorator<std::basic_string<CharT, Traits, Alloc>>>
    : std::true_type {};

template <class CharT, class Traits> struct is_string<std::basic_string_view<CharT, Traits>> : std::true_type {};

template <typename T> constexpr bool is_string_v = is_string<std::decay_t<T>>::value;

#endif // UTILITIES_STRING_H
