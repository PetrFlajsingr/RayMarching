//
// Created by petr on 2/10/20.
//

#ifndef UTILITIES_OVERLOAD_H
#define UTILITIES_OVERLOAD_H

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

#endif // UTILITIES_OVERLOAD_H
