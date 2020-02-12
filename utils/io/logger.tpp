

template <typename T> constexpr LogLevel LoggerStreamModifiers::modifierToLogLevel() {
  if constexpr (std::is_same_v<T, LoggerStreamModifiers::verbose>) {
    return LogLevel::Verbose;
  }
  if constexpr (std::is_same_v<T, LoggerStreamModifiers::info>) {
    return LogLevel::Info;
  }
  if constexpr (std::is_same_v<T, LoggerStreamModifiers::status>) {
    return LogLevel::Status;
  }
  if constexpr (std::is_same_v<T, LoggerStreamModifiers::debug>) {
    return LogLevel::Debug;
  }
  if constexpr (std::is_same_v<T, LoggerStreamModifiers::warning>) {
    return LogLevel::Warning;
  }
  if constexpr (std::is_same_v<T, LoggerStreamModifiers::error>) {
    return LogLevel::Error;
  }
}

template <typename OutStream> std::string Logger<OutStream>::getTime() const {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  std::stringstream ss;
  ss << std::put_time(&tm, "%H-%M-%S");
  return ss.str();
}

template <typename OutStream> std::string Logger<OutStream>::levelToString(LogLevel level) const {
  switch (level) {
  case LogLevel::Verbose:
    return "";
  case LogLevel::Info:
    return "[INFO]";
  case LogLevel::Status:
    return "[STATUS]";
  case LogLevel::Debug:
    return "[DEBUG]";
  case LogLevel::Warning:
    return "[WARNING]";
  case LogLevel::Error:
    return "[ERROR]";
  }
  throw std::exception();
}

template <typename OutStream> std::string Logger<OutStream>::indent(unsigned int level) const {
  auto cnt = level * 2;
  return std::string(cnt, ' ');
}

template <typename OutStream> template <typename T>
void Logger<OutStream>::print(const T &value, unsigned int indentLevel) const {
  using namespace std::string_literals;
  if constexpr (is_iterable_v<std::decay_t<T>> && !is_string_v<std::decay_t<T>>) {
    print(indent(indentLevel));
    print("Container, size: "s + std::to_string(value.size()));
    print(" {\n");

    for (const auto &val : value) {
      print(indent(indentLevel + 1));
      print(val, indentLevel + 1);
      print(",\n");
    }
    print(indent(indentLevel));
    print("},\n");
  } else if constexpr (is_vec_specialisation_v<std::decay_t<T>>) {
    print(indent(indentLevel));
    print("glm::vec" + std::to_string(value.length()) + ":");
    print(" {");
    for (auto i = 0; i < value.length(); ++i) {
      print(value[i]);
      if (i < value.length() - 1) {
        print(", ");
      }
    }
    print("}");
  } else if constexpr (std::is_same_v<T, glm::mat4>) {
    print("glm::mat4:");
    print("{\n");
    for (auto y = 0; y < 4; ++y) {
      print("{");
      for (auto x = 0; x < 4; ++x) {
        print(value[x][y]);
        if (x < 3) {
          print(", ");
        }
      }
      print("}\n");
    }
    print("}");
  } else {
    if constexpr (std::is_same_v<std::decay_t<T>, bool>) {
      outputStream << (value ? "true" : "false");
    } else if constexpr (is_specialization_v<T, std::optional>) {
      if (value.has_value()) {
        print(value.value());
      } else {
        print("Optional has no value.");
      }
    } else {
      outputStream << value;
    }
  }
}

template <typename OutStream> Logger<OutStream>::Logger(OutStream &outputStream) : outputStream(outputStream) {}

template <typename OutStream> template <LogLevel Level, bool PrintTime, bool PrintNewLine, typename... T>
void Logger<OutStream>::log(const T &... message) const {
  if constexpr (Level != LogLevel::Verbose) {
    if constexpr (PrintTime) {
      outputStream << levelToString(Level) + " " + getTime() + ": ";
    } else {
      outputStream << levelToString(Level) + ": ";
    }
  }
  (print(message), ...);
  if (PrintNewLine) {
    outputStream << std::endl;
  }
}

template <typename OutStream> void Logger<OutStream>::startTime() { startTimeMs = now<std::chrono::nanoseconds>(); }

template <typename OutStream> void Logger<OutStream>::endTime() { endTimeMs = now<std::chrono::nanoseconds>(); }

template <typename OutStream> void Logger<OutStream>::printElapsedTime() {
  auto tmp = endTimeMs - startTimeMs;
  log<LogLevel::Verbose>("Time elapsed: " + std::to_string(tmp.count()) + " ns");
}

template <typename OutStream> void Logger<OutStream>::setDefaultLevel(LogLevel logLevel) { defaultLevel = logLevel; }

template <typename OutStream> void Logger<OutStream>::setDefaultPrintTime(bool printTime) { defaultPrintTime = printTime; }

template <typename OutStream> template <typename T> auto Logger<OutStream>::operator<<(const T &rhs) {
  if constexpr (LoggerStreamModifiers::is_logger_stream_modifier_v<T>) {
    return OutOperator<LoggerStreamModifiers::modifierToLogLevel<T>()>(*this);
  } else if constexpr (LoggerStreamModifiers::is_logger_flusher_v<T>) {
    outputStream.flush();
    return *this;
  } else {
    log<LogLevel::Info, true, false>(rhs);
    return OutOperator<LogLevel::Verbose>(*this);
  }
}

template <typename OutStream> template <typename Callable, typename Resolution>
void Logger<OutStream>::measure(Callable &&callable, unsigned int iterations, std::string_view name) {
  using namespace MakeRange;
  static_assert(std::is_invocable_v<Callable>, "Callable must be invokable.");
  static_assert(is_duration_v<Resolution>, "Resolution must be duration type.");
  const auto startTime = now<Resolution>();
  for ([[maybe_unused]] auto i : until(0, iterations)) {
    callable();
  }
  const auto endTime = now<Resolution>();
  const auto totalTime = endTime - startTime;
  log<LogLevel::Verbose>("Measure time for: '", name, "', ", iterations, " iterations.");
  log<LogLevel::Verbose>("Total: ", totalTime.count(), " ", durationToString<Resolution>());
  log<LogLevel::Verbose>("Average: ", totalTime.count() / static_cast<float>(iterations), " ", durationToString<Resolution>());
}

template <typename OutStream> template <typename T> void Logger<OutStream>::remember(std::string_view key, const T &value) {
  memory[std::string(key)] = value;
}

template <typename OutStream> template <typename T> T Logger<OutStream>::recall(std::string_view key) {
  if (memory.count(std::string(key)) == 0) {
    return T{};
  } else {
    return std::any_cast<T>(memory[std::string(key)]);
  }
}

template <typename OutStream> template <LogLevel logLevel> Logger<OutStream>::OutOperator<logLevel>::OutOperator(Logger &logger)
    : logger(logger) {}

template <typename OutStream> template <LogLevel logLevel> template <typename T>
auto Logger<OutStream>::OutOperator<logLevel>::operator<<(const T &rhs) {
  if constexpr (LoggerStreamModifiers::is_logger_flusher_v<T>) {
    logger.outputStream.flush();
  } else if constexpr (logLevel == LogLevel::Verbose) {
    logger.log<logLevel, false, false>(rhs);
    return *this;
  } else {
    logger.log<logLevel, false, false>(rhs);
    return OutOperator<LogLevel::Verbose>(logger);
  }
}