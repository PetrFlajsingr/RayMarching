

//
// Created by Petr on 18.11.2019.
//

#ifndef UTILITIES_CACHEDPROPERTY_H
#define UTILITIES_CACHEDPROPERTY_H

#include <functional>
#include <utility>

template <typename T, bool LazyInit = true> class CachedProperty {
public:
  using Calculate = std::function<T()>;
  using Predicate = std::function<bool()>;

  explicit CachedProperty(Calculate calc);

  CachedProperty(Predicate pred, Calculate calc);

  CachedProperty(const CachedProperty &) = delete;

  CachedProperty(CachedProperty &&) = delete;

  operator const T &();

  T *operator->();

  T getCopy();

  const T &getRef();

  void setInitialised(bool initialised);

private:
  T cached;
  Predicate predicate;
  Calculate calculate;
  bool initialised = false;

  void checkAndRecalculate();
};

template <typename T> std::ostream &operator<<(std::ostream &stream, CachedProperty<T> &other);

#include "CachedProperty.tpp"

#endif // UTILITIES_CACHEDPROPERTY_H
