//
// Created by petr on 10/31/19.
//

#include "exceptions.h"
using namespace std::string_literals;
exc::Throwable::Throwable(std::string_view msg, std::experimental::source_location srcLoc)
    : what_(std::string(msg) + "\nThrown from file: " + srcLoc.file_name() + "\nfunction: " + srcLoc.function_name() +
            "\nline: " + std::to_string(srcLoc.line()) + "\ncolumn: " + std::to_string(srcLoc.column())) {}

const char *exc::Throwable::what() const noexcept { return what_.data(); }

exc::Error::Error(std::string_view msg, std::experimental::source_location srcLoc) : Throwable(msg, srcLoc) {}
exc::Exception::Exception(std::string_view msg, std::experimental::source_location srcLoc) : Throwable(msg, srcLoc) {}
exc::ProgrammingError::ProgrammingError(std::string_view msg, std::experimental::source_location srcLoc)
    : Error("FIX THIS: "s + std::string(msg), srcLoc) {}
exc::InternalError::InternalError(std::string_view msg, std::experimental::source_location srcLoc) : Error(msg, srcLoc) {}
