//
// Created by Petr on 18.11.2019.
//

template <typename T, bool LazyInit> CachedProperty<T, LazyInit>::CachedProperty(CachedProperty::Calculate calc)
    : CachedProperty([] { return true; }, calc) {}

template <typename T, bool LazyInit>
CachedProperty<T, LazyInit>::CachedProperty(CachedProperty::Predicate pred, CachedProperty::Calculate calc)
    : predicate(std::move(pred)), calculate(calc) {
  if constexpr (!LazyInit) {
    checkAndRecalculate();
  }
}

template <typename T, bool LazyInit> CachedProperty<T, LazyInit>::operator const T &() {
  checkAndRecalculate();
  return cached;
}

template <typename T, bool LazyInit> T *CachedProperty<T, LazyInit>::operator->() {
  checkAndRecalculate();
  return &cached;
}

template <typename T, bool LazyInit> T CachedProperty<T, LazyInit>::getCopy() {
  checkAndRecalculate();
  return cached;
}

template <typename T, bool LazyInit> const T &CachedProperty<T, LazyInit>::getRef() {
  checkAndRecalculate();
  return cached;
}

template <typename T, bool LazyInit> void CachedProperty<T, LazyInit>::setInitialised(bool initialised) {
  this->initialised = initialised;
}

template <typename T, bool LazyInit> void CachedProperty<T, LazyInit>::checkAndRecalculate() {
  if (predicate() || !initialised) {
    cached = calculate();
    initialised = true;
  }
}

template <typename T> std::ostream &operator<<(std::ostream &stream, CachedProperty<T> &other) {
  stream << other.getRef();
  return stream;
}
