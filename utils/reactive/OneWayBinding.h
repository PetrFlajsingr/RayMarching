//
// Created by petr on 11/21/19.
//

#ifndef UTILITIES_ONEWAYBINDING_H
#define UTILITIES_ONEWAYBINDING_H

#include <functional>
#include <include/observable/value.hpp>

template <typename T, typename U> struct OneWayBinding {
  using TransformAtoBfnc = std::function<T(const U &)>;
  using UnsubscriberB =
      decltype(std::declval<observable::value<U>>().subscribe(std::declval<std::function<void(const U &val)>>()));
  template <typename F1 = TransformAtoBfnc> OneWayBinding(observable::value<T> &a, observable::value<U> &b,
                                                          F1 transformA = TransformAtoBfnc{[](const U &val) { return T(val); }})
      : a(a), b(b), tA(transformA) {
    auto fncAtoB = [this](const T &val) { this->a = tA(val); };
    unsubscriberB = b.subscribe(fncAtoB);
  }

  ~OneWayBinding() { unsubscriberB.unsubscribe(); }

  observable::value<T> &a;
  observable::value<U> &b;

  UnsubscriberB unsubscriberB;

  TransformAtoBfnc tA;
};

template <typename T, typename U> struct OneWayBindingVal {
  using TransformAtoBfnc = std::function<U(const T &)>;
  using UnsubscriberB =
      decltype(std::declval<observable::value<U>>().subscribe(std::declval<std::function<void(const U &val)>>()));
  template <typename F1 = TransformAtoBfnc>
  OneWayBindingVal(T &a, observable::value<T> &b, F1 transformA = TransformAtoBfnc{[](const T &val) -> U { return U(val); }})
      : a(a), b(b), tA(transformA) {
    unsubscriberB = b.subscribe([this](const U &val) { this->a = val; });
  }

  ~OneWayBindingVal() { unsubscriberB.unsubscribe(); }

  T &a;
  observable::value<U> &b;

  UnsubscriberB unsubscriberB;

  TransformAtoBfnc tA;
};

#endif // UTILITIES_ONEWAYBINDING_H
