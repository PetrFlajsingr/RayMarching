//
// Created by Petr on 18.11.2019.
//

#ifndef UTILITIES_ISIN_H
#define UTILITIES_ISIN_H

template <typename T, typename Container> bool isIn(T value, Container &&container) {
  return std::find_if(container.begin(), container.end(), [value](const T &val) { return val == value; }) != container.end();
}

template <typename T> bool isIn(T value, std::initializer_list<T> &&container) {
  return std::find_if(container.begin(), container.end(), [value](const T &val) { return val == value; }) != container.end();
}

#endif // UTILITIES_ISIN_H
