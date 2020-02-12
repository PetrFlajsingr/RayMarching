//
// Created by Petr on 19.11.2019.
//

#ifndef UTILITIES_LOC_ASSERT_H
#define UTILITIES_LOC_ASSERT_H

#include "io/print.h"
#include <experimental/source_location>

inline void loc_assert(bool cond, std::string msg = "",
                       std::experimental::source_location sourceLocation = std::experimental::source_location::current()) {
  if (!cond) {
    printErr("Assert error on ", sourceLocation.file_name(), ", ", sourceLocation.function_name(), ", ", sourceLocation.line());
    printErr(msg);
    std::terminate();
  }
}

#endif // UTILITIES_LOC_ASSERT_H
