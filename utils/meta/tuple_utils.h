//
// Created by Petr on 09.12.2019.
//

#ifndef UTILITIES_TUPLE_TUILS_H
#define UTILITIES_TUPLE_TUILS_H

namespace details {
template <typename Int, typename, Int Begin> struct increasing_integer_range;

template <typename Int, Int... N, Int Begin> struct increasing_integer_range<Int, std::integer_sequence<Int, N...>, Begin>
    : std::integer_sequence<Int, N + Begin...> {};

template <typename Int, typename, Int Begin> struct decreasing_integer_range;

template <typename Int, Int... N, Int Begin> struct decreasing_integer_range<Int, std::integer_sequence<Int, N...>, Begin>
    : std::integer_sequence<Int, Begin - N...> {};
} // namespace details

template <typename Int, Int Begin, Int End, bool Increasing = (Begin < End)> struct integer_range;

template <typename Int, Int Begin, Int End> struct integer_range<Int, Begin, End, true>
    : details::increasing_integer_range<Int, std::make_integer_sequence<Int, End - Begin>, Begin> {};

template <typename Int, Int Begin, Int End> struct integer_range<Int, Begin, End, false>
    : details::decreasing_integer_range<Int, std::make_integer_sequence<Int, Begin - End>, Begin> {};

template <std::size_t Begin, std::size_t End> using index_range = integer_range<std::size_t, Begin, End>;

template <size_t SN, size_t DN, class TSrc, class TDest, class Func>
void tuple_call_assign(TSrc &&source, TDest &target, Func f) {
  std::get<DN>(target) = f(std::get<SN>(std::forward<TSrc>(source)));
}

template <size_t From, size_t To, class TSrc, class TDest, class Func, size_t... Is, size_t... DIs>
void tuple_transform(TSrc &&source, TDest &target, Func f, std::index_sequence<Is...>, std::index_sequence<DIs...>) {
  using expander = int[];
  (void)expander{0, (tuple_call_assign<Is, DIs>(std::forward<TSrc>(source), target, f), 0)...};
}

template <size_t From, size_t To, size_t FromDest, class TSrc, class TDest, class Func>
void tuple_transform(TSrc &&source, TDest &target, Func f) {
  static_assert(To > From, "Range must be increasing");
  static_assert(To <= std::tuple_size<std::decay_t<TSrc>>::value + 1, "Range must be valid for source tuple");
  constexpr size_t RangeSize = To - From;
  static_assert(FromDest + RangeSize <= std::tuple_size<std::decay_t<TDest>>::value, "Range must be valid for target tuple");

  tuple_transform<From, To>(std::forward<TSrc>(source), target, f, index_range<From, To>(),
                            index_range<FromDest, FromDest + RangeSize>());
}

template <class Func, class Tuple, size_t... Is> void for_each_in_tuple(Func f, Tuple &&tuple, std::index_sequence<Is...>) {
  using expander = int[];
  (void)expander{0, ((void)f(std::get<Is>(std::forward<Tuple>(tuple))), 0)...};
}

template <class Func, class Tuple> void for_each_in_tuple(Func f, Tuple &&tuple) {
  for_each_in_tuple(f, std::forward<Tuple>(tuple), std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>());
}

template <typename Func, typename Tuple1, typename Tuple2, size_t... Is>
void for_each_in_tuple_pair(Func f, Tuple1 &&tuple, Tuple2 &&tuple2, std::index_sequence<Is...>) {
  using expander = int[];
  (void)expander{0, ((void)f(std::get<Is>(std::forward<Tuple1>(tuple)), std::get<Is>(std::forward<Tuple2>(tuple2))), 0)...};
}

template <typename Func, typename Tuple1, typename Tuple2> void for_each_in_tuple_pair(Func f, Tuple1 &&tuple, Tuple2 &&tuple2) {
  for_each_in_tuple_pair(f, std::forward<Tuple1>(tuple), std::forward<Tuple2>(tuple2),
                         std::make_index_sequence<std::tuple_size<std::decay_t<Tuple1>>::value>());
}

#endif // UTILITIES_TUPLE_TUILS_H
