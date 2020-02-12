//
// Created by Petr on 18.11.2019.
//

#ifndef UTILITIES_RANGE_H
#define UTILITIES_RANGE_H

#include <array>
#include <cassert>
#include <functional>
#include <meta/meta.h>

template <typename T> class Range {
public:
  struct iterator {
    using value_type = T;
    using reference = T &;
    using pointer = T *;
    using const_pointer = const T *;
    using difference_type = int;
    using iterator_category = std::input_iterator_tag;

    constexpr iterator() = default;

    constexpr iterator(value_type value, value_type step, value_type endValue, bool up);

    constexpr iterator(const iterator &other);

    constexpr iterator &operator=(const iterator &other);

    bool operator==(const iterator &rhs) const;

    bool operator!=(const iterator &rhs) const;

    value_type operator*() const;

    const_pointer operator->();

    iterator &operator++();

    iterator operator++(int);

    T value;
    T step;
    T endValue;
    bool up;
  };

public:
  template <typename U = T, typename V = T> constexpr Range(T start, U end, V step = V{1});

  constexpr iterator begin() const;

  constexpr iterator end() const;

private:
  const T _start;
  const T _end;
  const T _step;
};

template <typename T, unsigned int Dimensions> class MultiDimRange {

  static_assert(Dimensions > 1);

public:
  template <typename U> using container_type = std::array<U, Dimensions>;
  using value_type = container_type<T>;

  struct iterator {
    using reference = value_type &;
    using pointer = value_type *;
    using difference_type = int;
    using iterator_category = std::input_iterator_tag;

    iterator() = default;

    iterator(value_type value, value_type step, value_type startValue, value_type endValue);

    iterator(const iterator &other);

    iterator &operator=(const iterator &other);

    bool operator==(const iterator &rhs) const;

    bool operator!=(const iterator &rhs) const;

    value_type operator*() const;

    value_type *operator->();

    iterator &operator++();

    iterator operator++(int);

    value_type value;
    value_type step;
    value_type startValue;
    value_type endValue;
    container_type<std::function<bool(const T &, const T &)>> comp;
  };

public:
  template <typename U = T> MultiDimRange(value_type start, value_type end, value_type step);

  iterator begin() const;

  iterator end() const;

private:
  const value_type _start;
  const value_type _end;
  const value_type _step;
};

namespace MakeRange {
template <typename T, typename U = T, typename V = T> constexpr Range<T> until(T start, U end, V step = T{1});

template <typename T, typename U = T, typename V = T> constexpr Range<T> to(T start, U end, V step = T{1});

template <typename T, typename U = T, typename V = T> constexpr Range<T> downTo(T start, U end, V step = T{1});

template <typename T, typename U = T, typename V = T> constexpr Range<T> downUntil(T start, U end, V step = T{1});

template <
    typename T, typename U, typename V = T,
    typename ValueType = typename std::conditional_t<
        any_of_v<std::is_floating_point, T, U, V>, highest_precision_t<T, U, V>,
        std::conditional_t<any_of_v<std::is_signed, T, U, V>, largest_signed_integer_t<T, U, V>, largest_integer_t<T, U, V>>>>
constexpr Range<ValueType> range(T start, U end, V step = T{1});

template <typename T> constexpr Range<T> range(T start);

template <typename T, unsigned int Dimensions>
MultiDimRange<T, Dimensions> range(typename MultiDimRange<T, Dimensions>::value_type start,
                                   typename MultiDimRange<T, Dimensions>::value_type end,
                                   typename MultiDimRange<T, Dimensions>::value_type step);

template <typename T, unsigned int Dimensions>
MultiDimRange<T, Dimensions> range(typename MultiDimRange<T, Dimensions>::value_type end);
} // namespace MakeRange

#include "Range.tpp"

#endif // UTILITIES_RANGE_H
