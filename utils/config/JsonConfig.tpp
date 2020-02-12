

template <typename... Keys>
std::optional<nlohmann::json::iterator> findJsonNode(nlohmann::json &container, const Keys &... keys) {
  std::vector<std::string> tmpKeys{keys...};
  nlohmann::json::iterator iter;
  for (unsigned int i : until(0, tmpKeys.size())) {
    if (i == 0) {
      if (iter = container.find(tmpKeys[i]); iter == container.end()) {
        return std::nullopt;
      }
    } else {
      const auto tmpEnd = iter->end();
      if (iter = iter->find(tmpKeys[i]); iter == tmpEnd) {
        return std::nullopt;
      }
    }
  }
  return iter;
}

template <typename T, typename... Keys> void setJsonNode(nlohmann::json &container, const T &value, const Keys &... keys) {
  std::vector<std::string> tmpKeys{keys...};
  auto tmp = container;
  for (unsigned int i : until(0, tmpKeys.size() - 1)) {
    if (!tmp.contains(tmpKeys[i])) {
      tmp[tmpKeys[i]] = nlohmann::json::object();
    }
    tmp = tmp[tmpKeys[i]];
  }
  tmp[tmpKeys.back()] = value;
}

template <typename... Keys>
bool ConfigContainerTraits<nlohmann::json>::contains(nlohmann::json &container, const Keys &... keys) {
  return findJsonNode(container, keys...).has_value();
}
template <typename T, typename... Keys>
std::optional<T> ConfigContainerTraits<nlohmann::json>::find(nlohmann::json &container, const Keys &... keys) {
  if (auto iter = findJsonNode(container, keys...); iter.has_value()) {
    return **iter;
  }
  return std::nullopt;
}
template <typename T, typename... Keys>
void ConfigContainerTraits<nlohmann::json>::set(nlohmann::json &container, const T &value, const Keys &... keys) {
  setJsonNode(container, value, keys...);
}

nlohmann::json ConfigLoader<nlohmann::json>::load(std::string_view path) {
  std::ifstream stream{std::string(path)};
  nlohmann::json result;
  stream >> result;
  return result;
}

void ConfigSaver<nlohmann::json>::save(nlohmann::json &config, std::string_view path) {
  std::ofstream stream{std::string(path)};
  stream << config;
}