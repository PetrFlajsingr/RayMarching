
template <typename DataContainer, bool ReadOnly, typename Key> Config<DataContainer, ReadOnly, Key>::Config(std::string_view path)
    : path(std::string(path)) {
  reload();
}

template <typename DataContainer, bool ReadOnly, typename Key> template <typename T, typename... Keys>
std::optional<T> Config<DataContainer, ReadOnly, Key>::get(const Keys &... keys) {
  return container_traits::template find<T>(data, keys...);
}

template <typename DataContainer, bool ReadOnly, typename Key> template <typename T, typename... Keys>
T Config<DataContainer, ReadOnly, Key>::getDefault(const T &defaultValue, const Keys &... keys) {
  if (auto found = get<T>(keys...); found.has_value()) {
    return found.value();
  }
  return defaultValue;
}

template <typename DataContainer, bool ReadOnly, typename Key> template <typename T, typename, typename... Keys>
Config<DataContainer, ReadOnly, Key> &Config<DataContainer, ReadOnly, Key>::set(const T &value, const Keys &... keys) {
  container_traits::set(data, value, keys...);
  return *this;
}

template <typename DataContainer, bool ReadOnly, typename Key> template <typename>
void Config<DataContainer, ReadOnly, Key>::save() {
  ConfigSaver<DataContainer> saver;
  saver.save(data, path);
}

template <typename DataContainer, bool ReadOnly, typename Key> void Config<DataContainer, ReadOnly, Key>::reload() {
  ConfigLoader<DataContainer> loader;
  data = loader.load(path);
}

template <typename DataContainer, bool ReadOnly, typename Key> template <typename... Keys>
bool Config<DataContainer, ReadOnly, Key>::contains(const Keys &... keys) {
  return container_traits::contains(data, keys...);
}
