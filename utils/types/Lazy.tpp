
template <typename T> Lazy<T>::Lazy(Lazy::CalcFunction &&calcFunction) : calcFunction(calcFunction) {}
template <typename T> const T &Lazy<T>::value() {
  if (!data.has_value()) {
    data = calcFunction();
  }
  return data.value();
}

template <typename T> void Lazy<T>::invalidate() { data = std::nullopt; }

template <typename T> std::ostream &operator<<(std::ostream &stream, Lazy<T> &other) {
  stream << other.value();
  return stream;
}

template <typename T> Lazy<T>::operator const T &() { return value(); }