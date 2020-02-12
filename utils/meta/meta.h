//
// Created by petr on 10/28/19.
//

#ifndef UTILITIES_META_H
#define UTILITIES_META_H

#include "chrono.h"
#include "iterable.h"
#include "specializations.h"
#include "string.h"
#include <chrono>
#include <glm/glm.hpp>
#include <type_traits>

template <typename T1, typename... T> struct first_of { using type = T1; };
template <typename... T> using first_of_t = typename first_of<T...>::type;

template <typename T, typename... Types> constexpr bool is_one_of_v = (std::is_same_v<T, Types> || ...);

template <template <class> typename T, typename... Types> constexpr bool any_of_v = (T<Types>::value || ...);

template <template <class> typename T, typename... Types> constexpr bool all_of_v = (T<Types>::value && ...);

template <typename T> struct tag { using type = T; };
template <typename... Ts> struct last_of { using type = typename decltype((tag<Ts>{}, ...))::type; };
template <typename... T> using last_of_t = typename last_of<T...>::type;

template <typename T> struct is_char {
  static constexpr bool value = std::is_same_v<T, char> || std::is_same_v<T, unsigned char> || std::is_same_v<T, signed char> ||
                                std::is_same_v<T, wchar_t> || std::is_same_v<T, char16_t> || std::is_same_v<T, char32_t>;
};
template <typename T> constexpr bool is_char_v = is_char<T>::value;

template <typename... T> using highest_precision_t = std::conditional_t<
    is_one_of_v<long double, T...>, long double,
    std::conditional_t<is_one_of_v<double, T...>, double, std::conditional_t<is_one_of_v<float, T...>, float, void>>>;

template <typename... T> using largest_integer_t = std::conditional_t<
    is_one_of_v<long long, T...>, long long,
    std::conditional_t<
        is_one_of_v<unsigned long long, T...>, unsigned long long,
        std::conditional_t<
            is_one_of_v<long, T...>, long,
            std::conditional_t<
                is_one_of_v<unsigned long, T...>, unsigned long,
                std::conditional_t<
                    is_one_of_v<int, T...>, int,
                    std::conditional_t<
                        is_one_of_v<unsigned int, T...>, unsigned int,
                        std::conditional_t<
                            is_one_of_v<unsigned short, T...>, unsigned short,
                            std::conditional_t<
                                is_one_of_v<short, T...>, short,
                                std::conditional_t<
                                    is_one_of_v<char32_t, T...>, char32_t,
                                    std::conditional_t<
                                        is_one_of_v<char16_t, T...>, char16_t,
                                        std::conditional_t<
                                            is_one_of_v<wchar_t, T...>, wchar_t,
                                            std::conditional_t<is_one_of_v<signed char, T...>, signed char,
                                                               std::conditional_t<is_one_of_v<unsigned char, T...>, unsigned char,
                                                                                  std::conditional_t<is_one_of_v<char, T...>,
                                                                                                     char, void>>>>>>>>>>>>>>;

template <typename... T> using largest_signed_integer_t = std::conditional_t<
    is_one_of_v<long long, T...>, long long,
    std::conditional_t<
        is_one_of_v<long, T...>, long,
        std::conditional_t<
            is_one_of_v<int, T...>, int,
            std::conditional_t<
                is_one_of_v<short, T...>, short,
                std::conditional_t<
                    is_one_of_v<char32_t, T...>, char32_t,
                    std::conditional_t<
                        is_one_of_v<char16_t, T...>, char16_t,
                        std::conditional_t<is_one_of_v<wchar_t, T...>, wchar_t,
                                           std::conditional_t<is_one_of_v<signed char, T...>, signed char,
                                                              std::conditional_t<is_one_of_v<char, T...>, char, void>>>>>>>>>;
#endif // UTILITIES_META_H
