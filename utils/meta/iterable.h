//
// Created by Petr on 18.11.2019.
//

#ifndef UTILITIES_ITERABLE_H
#define UTILITIES_ITERABLE_H

#include <iterator>

template <typename T, typename = void> struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T> constexpr bool is_iterable_v = is_iterable<T>::value;

#endif // UTILITIES_ITERABLE_H
