//
// Created by Petr on 03.12.2019.
//

#ifndef UTILITIES_ZIP_H
#define UTILITIES_ZIP_H

#include "meta/meta.h"
#include "meta/tuple_utils.h"
#include <type_traits>
#include <utility>

template <typename... T> class Zip {
public:
  explicit Zip(const T &... vals);

  struct iterator {
    using value_type = std::tuple<std::decay_t<decltype(*std::declval<decltype(std::begin(std::declval<T>()))>())>...>;
    std::tuple<decltype(std::begin(std::declval<T>()))...> iters;

    explicit iterator(std::tuple<decltype(std::begin(std::declval<T>()))...> iters);

    iterator(const iterator &other) = default;
    iterator(iterator &&other) noexcept = default;
    iterator &operator=(const iterator &other) = default;
    iterator &operator=(iterator &&other) noexcept = default;

    bool operator==(const iterator &rhs) const;

    bool operator!=(const iterator &rhs) const;

    value_type operator*();

    iterator &operator++();

    const iterator operator++(int);
  };

  iterator begin();

  iterator end();

private:
  std::tuple<decltype(std::begin(std::declval<T>()))...> beginIters;
  std::tuple<decltype(std::end(std::declval<T>()))...> endIters;
};

template <typename... T> Zip<T...> zip(const T &... vals);

#include "Zip.tpp"

#endif // UTILITIES_ZIP_H
