//
// Created by petr on 11/21/19.
//

#ifndef UTILITIES_TWOWAYBINDING_H
#define UTILITIES_TWOWAYBINDING_H

#include <functional>
#include <include/observable/value.hpp>

template <typename T, typename U> struct TwoWayBinding {
  using TransformAtoBfnc = std::function<U(const T &)>;
  using TransformBtoAfnc = std::function<T(const U &)>;

  using UnsubscriberA =
      decltype(std::declval<observable::value<T>>().subscribe(std::declval<std::function<void(const T &val)>>()));
  using UnsubscriberB =
      decltype(std::declval<observable::value<U>>().subscribe(std::declval<std::function<void(const U &val)>>()));

  template <typename F1 = TransformAtoBfnc, typename F2 = TransformBtoAfnc>
  TwoWayBinding(observable::value<T> &a, observable::value<U> &b,
                F1 transformA = TransformAtoBfnc{[](const T &val) -> U { return U(val); }},
                F2 transformB = TransformBtoAfnc{[](const U &val) -> T { return T(val); }})
      : a(a), b(b), tA(std::move(transformA)), tB(std::move(transformB)) {

    auto fncAtoB = [this](const T &val) {
      if (avoidCycle) {
        return;
      }
      avoidCycle = true;
      this->b = tA(val);
      avoidCycle = false;
    };
    auto fncBtoA = [this](const U &val) {
      if (avoidCycle) {
        return;
      }
      avoidCycle = true;
      this->a = tB(val);
      avoidCycle = false;
    };

    unsubscriberA = a.subscribe(fncAtoB);
    unsubscriberB = b.subscribe(fncBtoA);
  }

  ~TwoWayBinding() {
    unsubscriberA.unsubscribe();
    unsubscriberB.unsubscribe();
  }
  observable::value<T> &a;
  observable::value<U> &b;

  TransformAtoBfnc tA;
  TransformBtoAfnc tB;

  UnsubscriberA unsubscriberA;
  UnsubscriberB unsubscriberB;

  bool avoidCycle = false;
};

#endif // UTILITIES_TWOWAYBINDING_H
