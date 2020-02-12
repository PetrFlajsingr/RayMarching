//
// Created by Petr on 18.11.2019.
//

#ifndef UTILITIES_CONFIG_H
#define UTILITIES_CONFIG_H

#include <optional>
#include <string>
#include <type_traits>

/*
 * Usage:
 * Specialize ConfigContainerTraits for your container
 * Specialize ConfigLoader for your container
 * Specialize ConfigSaver for your container (if you want to allow saving)
 *
 * template <bool ReadOnly>
 * using MyContainerConfig = Config<MyContainer, ReadOnly>
 */

template <typename Container> struct ConfigContainerTraits {
  template <typename T, typename... Keys> static std::optional<T> find(Container &, const Keys &... value);

  template <typename T, typename... Keys> static bool contains(Container &, const Keys &... keys);

  template <typename T, typename... Keys> static void set(Container &, const T &value, const Keys &... key);
};

template <typename Container> struct ConfigLoader { Container load(std::string_view path); };

template <typename Container> struct ConfigSaver { void save(Container &config, std::string_view path); };

template <typename DataContainer, bool ReadOnly, typename Key = std::string> class Config {
  using container_traits = ConfigContainerTraits<DataContainer>;

public:
  /**
   * Load config from given path.
   */
  explicit Config(std::string_view path);
  /**
   * Get value by key, return std::nullopt for non-existent value.
   * @tparam T requested value type
   */
  template <typename T, typename... Keys> std::optional<T> get(const Keys &... keys);
  /**
   * Get value by key, return defaultValue for non-existent value.
   * @tparam T requested value type
   */
  template <typename T, typename... Keys> T getDefault(const T &defaultValue, const Keys &... keys);
  /**
   * Set value for key. Allowed only if config is not in ReadOnly mode.
   */
  template <typename T, typename = std::enable_if_t<!ReadOnly>, typename... Keys>
  Config &set(const T &value, const Keys &... keys);

  template <typename... Keys> bool contains(const Keys &... keys);
  /**
   * Save config to file. Allowed only if config is not in ReadOnly mode.
   */
  template <typename = std::enable_if_t<!ReadOnly>> void save();
  /**
   * Reload config data from disk.
   */
  void reload();

private:
  DataContainer data;
  std::string path;
};

#include "Config.tpp"

#endif // UTILITIES_CONFIG_H
