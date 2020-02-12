//
// Created by Petr on 18.11.2019.
//

#ifndef UTILITIES_SPECIALIZATIONS_H
#define UTILITIES_SPECIALIZATIONS_H

#include <type_traits>

template <class T, template <class...> class Template> struct is_specialization : std::false_type {};

template <template <class...> class Template, class... Args> struct is_specialization<Template<Args...>, Template>
    : std::true_type {};

template <typename T, template <class...> class U> static inline constexpr bool is_specialization_v =
    is_specialization<T, U>::value;

#endif // UTILITIES_SPECIALIZATIONS_H
