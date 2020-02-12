//
// Created by Petr Flajsingr on 2019-01-25.
//

#ifndef UTILITIES_LOGGER_H
#define UTILITIES_LOGGER_H

#include "meta/glm.h"
#include "meta/meta.h"
#include "time/now.h"
#include "types/Range.h"
#include <any>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

/**
 * Types of log messages
 */
enum class LogLevel { Verbose, Info, Status, Debug, Warning, Error };

namespace LoggerStreamModifiers {
struct verbose {};
struct info {};
struct status {};
struct debug {};
struct warning {};
struct error {};
struct flush {};
struct out {};
struct err {};

template <typename T> constexpr LogLevel modifierToLogLevel();

template <typename T> constexpr bool is_logger_stream_modifier_v = is_one_of_v<T, verbose, info, status, debug, warning, error>;
template <typename T> constexpr bool is_logger_flusher_v = std::is_same_v<flush, T>;
template <typename T> constexpr bool is_stream_v = is_one_of_v<T, out, err>;
} // namespace LoggerStreamModifiers

template <typename OutStream> class Logger {
private:
  /**
   *
   * @return Current time as HH-MM-SS
   */
  [[nodiscard]] std::string getTime() const;

  [[nodiscard]] std::string levelToString(LogLevel level) const;

  std::chrono::nanoseconds startTimeMs;
  std::chrono::nanoseconds endTimeMs;

  LogLevel defaultLevel = LogLevel::Verbose;
  bool defaultPrintTime = true;

  OutStream &outputStream;

  [[nodiscard]] std::string indent(unsigned int level) const;

  template <typename T> void print(const T &value, unsigned int indentLevel = 0) const;

  std::unordered_map<std::string, std::any> memory;

public:
  explicit Logger(OutStream &outputStream);

  template <LogLevel Level, bool PrintTime = false, bool PrintNewLine = true, typename... T> void log(const T &... message) const;

  void startTime();

  void endTime();

  /**
   * Print time difference between endTime() and startTime() calls
   */
  void printElapsedTime();

  void setDefaultLevel(LogLevel logLevel);

  void setDefaultPrintTime(bool printTime);

  template <typename T> void remember(std::string_view key, const T &value);

  template <typename T> T recall(std::string_view key);

private:
  template <LogLevel logLevel> struct OutOperator {
    Logger &logger;

    explicit OutOperator(Logger &logger);

    template <typename T> auto operator<<(const T &rhs);
  };

public:
  template <typename T> auto operator<<(const T &rhs);

  template <typename Callable, typename Resolution = std::chrono::milliseconds>
  void measure(Callable &&callable, unsigned int iterations, std::string_view name = "");
};

#include "logger.tpp"

#endif // UTILITIES_LOGGER_H