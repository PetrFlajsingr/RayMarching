//
// Created by petr on 10/31/19.
//

#ifndef UTILITIES_EXCEPTIONS_H
#define UTILITIES_EXCEPTIONS_H

#include <exception>
#include <experimental/source_location>
#include <string>

namespace exc {
class Throwable : public std::exception {
public:
  explicit Throwable(std::string_view msg,
                     std::experimental::source_location srcLoc = std::experimental::source_location::current());
  [[nodiscard]] const char *what() const noexcept override;

protected:
  std::string what_;
};

class Error : public Throwable {
public:
  explicit Error(std::string_view msg, std::experimental::source_location srcLoc = std::experimental::source_location::current());
};
class Exception : public Throwable {
public:
  explicit Exception(std::string_view msg,
                     std::experimental::source_location srcLoc = std::experimental::source_location::current());
  ;
};

class ProgrammingError : public Error {
public:
  explicit ProgrammingError(std::string_view msg,
                            std::experimental::source_location srcLoc = std::experimental::source_location::current());
  ;
};

class InternalError : public Error {
public:
  explicit InternalError(std::string_view msg,
                         std::experimental::source_location srcLoc = std::experimental::source_location::current());
  ;
};

} // namespace exc

#endif // UTILITIES_EXCEPTIONS_H
