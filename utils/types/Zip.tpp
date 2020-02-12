
template <typename... T> Zip<T...>::Zip(const T &... vals) : beginIters(std::begin(vals)...), endIters(std::end(vals)...) {}

template <typename... T> typename Zip<T...>::iterator Zip<T...>::begin() { return iterator(beginIters); }

template <typename... T> typename Zip<T...>::iterator Zip<T...>::end() { return iterator(endIters); }

template <typename... T> Zip<T...>::iterator::iterator(std::tuple<decltype(std::begin(std::declval<T>()))...> iters)
    : iters(iters) {}

template <typename... T> bool Zip<T...>::iterator::operator==(const Zip::iterator &rhs) const {
  bool areSame = false;
  for_each_in_tuple_pair([&areSame](const auto &first, const auto &second) { areSame = areSame || first == second; }, iters,
                         rhs.iters);
  return areSame;
}

template <typename... T> bool Zip<T...>::iterator::operator!=(const Zip::iterator &rhs) const { return !(rhs == *this); }

template <typename... T> typename Zip<T...>::iterator::value_type Zip<T...>::iterator::operator*() {
  value_type result;
  tuple_transform<0, std::tuple_size_v<value_type>, 0>(iters, result, [](auto &it) { return *it; });
  return result;
}

template <typename... T> typename Zip<T...>::iterator &Zip<T...>::iterator::operator++() {
  for_each_in_tuple([](auto &val) { ++val; }, iters);
  return *this;
}

template <typename... T> const typename Zip<T...>::iterator Zip<T...>::iterator::operator++(int) {
  iterator result{*this};
  ++*this;
  return result;
}

template <typename... T> Zip<T...> zip(const T &... vals) { return std::move(Zip(vals...)); }
