//
// Created by Petr on 18.11.2019.
//

#ifndef UTILITIES_STRINGUTILS_H
#define UTILITIES_STRINGUTILS_H

#include <fmt/format.h>
#include <optional>
#include <ostream>
#include <vector>

namespace StringUtils {
template <template <class> typename ContainerOut = std::vector, typename StringType>
ContainerOut<StringType> split(const StringType &str, const StringType &delimiter);

template <typename To, typename StringType> std::optional<To> to(const StringType &str);

template <typename StringType> bool startsWith(const StringType &str, const StringType &searched);

template <typename StringType> bool endsWith(const StringType &str, const StringType &searched);

template <typename T, typename StringType> StringType from(const T &value) { return std::to_string(value); }

inline std::string getRandomString(size_t length) {
  auto randChar = []() -> char {
    const char charset[] = "0123456789"
                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                           "abcdefghijklmnopqrstuvwxyz";
    const size_t max_index = (sizeof(charset) - 1);
    return charset[rand() % max_index];
  };
  std::string str(length, 0);
  std::generate_n(str.begin(), length, randChar);
  return str;
}

inline std::string toLower(const std::string &str) {
  std::string result;
  std::transform(str.begin(), str.end(), std::back_inserter(result), ::tolower);
  return result;
}

inline std::string toUpper(const std::string &str) {
  std::string result;
  std::transform(str.begin(), str.end(), std::back_inserter(result), ::toupper);
  return result;
}

} // namespace StringUtils

template <typename StringType> class StringDecorator {
  using reference = typename StringType::reference;
  using const_reference = typename StringType::const_reference;
  using size_type = typename StringType::size_type;
  using CharT = typename StringType::value_type;
  using iterator = typename StringType::iterator;
  using const_iterator = typename StringType::const_iterator;
  using reverse_iterator = typename StringType::reverse_iterator;
  using const_reverse_iterator = typename StringType::const_reverse_iterator;

public:
  static const size_type npos = StringType::npos;

  template <typename... Args> explicit StringDecorator(const Args &... args);

  static StringDecorator Random(size_type length);

  template <typename T> inline StringDecorator &operator=(const T &other);

  template <typename T> inline StringDecorator &operator=(T &&other);

  template <typename T, typename = typename std::enable_if_t<std::is_convertible_v<StringType, T>>> inline operator T() const;

  inline reference at(size_type pos);

  inline const_reference at(size_type pos) const;

  inline reference operator[](size_type pos);

  inline const_reference operator[](size_type pos) const;

  inline CharT &front();

  inline const CharT &front() const;

  inline CharT &back();

  inline const CharT &back() const;

  inline CharT *data();

  inline const CharT *data() const;

  inline iterator begin() noexcept;

  inline const_iterator begin() const noexcept;

  inline const_iterator cbegin() const noexcept;

  inline iterator end() noexcept;

  inline const_iterator end() const noexcept;

  inline const_iterator cend() const noexcept;

  inline reverse_iterator rbegin() noexcept;

  inline const_reverse_iterator rbegin() const noexcept;

  inline const_reverse_iterator crbegin() const noexcept;

  inline reverse_iterator rend() noexcept;

  inline const_reverse_iterator rend() const noexcept;

  inline const_reverse_iterator crend() const noexcept;

  inline bool empty() const noexcept;

  inline size_type size() const noexcept;

  inline size_type length() const noexcept;

  inline size_type max_size() const noexcept;

  inline void reserve(size_type new_cap);

  inline size_type capacity() const noexcept;

  inline void shrink_to_fit();

  inline void clear() noexcept;

  inline void swap(StringDecorator &other) noexcept;

  inline void resize(size_type count);

  inline void resize(size_type count, CharT ch);

  inline size_type copy(CharT *dest, size_type count, size_type pos = 0) const;

  inline StringDecorator substr(size_type pos = 0, size_type count = npos);

  template <typename T> inline StringDecorator &operator+=(const T &t);

  inline void pop_back();

  inline void push_back(CharT ch);

  inline StringDecorator &erase(size_type index = 0, size_type count = npos);

  inline iterator erase(const_iterator position);

  inline iterator erase(const_iterator first, const_iterator last);

  inline size_type find(const StringDecorator &str, size_type pos = 0) const noexcept;

  inline size_type find(const StringType &str, size_type pos = 0) const noexcept;

  inline size_type find(const CharT *s, size_type pos, size_type count) const;

  inline size_type find(const CharT *s, size_type pos = 0) const;

  inline size_type find(CharT ch, size_type pos = 0) const noexcept;

  template <class T> inline size_type find(const T &t, size_type pos = 0) const noexcept;

  inline size_type rfind(const StringDecorator &str, size_type pos = 0) const noexcept;

  inline size_type rfind(const StringType &str, size_type pos = 0) const noexcept;

  inline size_type rfind(const CharT *s, size_type pos, size_type count) const;

  inline size_type rfind(const CharT *s, size_type pos = 0) const;

  inline size_type rfind(CharT ch, size_type pos = 0) const noexcept;

  template <typename T> inline size_type rfind(const T &t, size_type pos = 0) const noexcept;

  inline size_type find_first_of(const StringType &str, size_type pos = 0) const noexcept;

  inline size_type find_first_of(const StringDecorator &str, size_type pos = 0) const noexcept;

  inline size_type find_first_of(const CharT *s, size_type pos, size_type count) const;

  inline size_type find_first_of(const CharT *s, size_type pos = 0) const;

  inline size_type find_first_of(CharT ch, size_type pos = 0) const noexcept;

  template <class T> inline size_type find_first_of(const T &t, size_type pos = 0) const noexcept;

  inline size_type find_first_not_of(const StringDecorator &str, size_type pos = 0) const noexcept;

  inline size_type find_first_not_of(const StringType &str, size_type pos = 0) const noexcept;

  inline size_type find_first_not_of(const CharT *s, size_type pos, size_type count) const;

  inline size_type find_first_not_of(const CharT *s, size_type pos = 0) const;

  inline size_type find_first_not_of(CharT ch, size_type pos = 0) const noexcept;

  template <class T> inline size_type find_first_not_of(const T &t, size_type pos = 0) const noexcept;

  inline size_type find_last_of(const StringDecorator &str, size_type pos = npos) const noexcept;

  inline size_type find_last_of(const StringType &str, size_type pos = npos) const noexcept;

  inline size_type find_last_of(const CharT *s, size_type pos, size_type count) const;

  inline size_type find_last_of(const CharT *s, size_type pos = npos) const;

  inline size_type find_last_of(CharT ch, size_type pos = npos) const noexcept;

  template <class T> inline size_type find_last_of(const T &t, size_type pos = npos) const noexcept;

  inline size_type find_last_not_of(const StringDecorator &str, size_type pos = npos) const noexcept;

  inline size_type find_last_not_of(const StringType &str, size_type pos = npos) const noexcept;

  inline size_type find_last_not_of(const CharT *s, size_type pos, size_type count) const;

  inline size_type find_last_not_of(const CharT *s, size_type pos = npos) const;

  inline size_type find_last_not_of(CharT ch, size_type pos = npos) const noexcept;

  template <class T> inline size_type find_last_not_of(const T &t, size_type pos = npos) const noexcept;

  inline StringDecorator &insert(size_type index, size_type count, CharT ch);

  inline StringDecorator &insert(size_type index, const CharT *s);

  inline StringDecorator &insert(size_type index, const CharT *s, size_type count);

  inline StringDecorator &insert(size_type index, const StringDecorator &str);

  inline StringDecorator &insert(size_type index, const StringType &str);

  inline StringDecorator &insert(size_type index, const StringDecorator &str, size_type index_str, size_type count = npos);

  inline StringDecorator &insert(size_type index, const StringType &str, size_type index_str, size_type count = npos);

  inline iterator insert(const_iterator pos, CharT ch);

  inline iterator insert(const_iterator pos, size_type count, CharT ch);

  template <typename InputIt> inline iterator insert(const_iterator pos, InputIt first, InputIt last);

  inline iterator insert(const_iterator pos, std::initializer_list<CharT> ilist);

  template <typename T> inline StringDecorator &insert(size_type pos, const T &t);

  template <typename T> inline StringDecorator &insert(size_type index, const T &t, size_type index_str, size_type count = npos);

  inline StringDecorator &append(size_type count, CharT ch);

  inline StringDecorator &append(const StringDecorator &str);

  inline StringDecorator &append(const StringType &str);

  inline StringDecorator &append(const StringDecorator &str, size_type pos, size_type count = npos);

  inline StringDecorator &append(const StringType &str, size_type pos, size_type count = npos);

  inline StringDecorator &append(const CharT *s, size_type count);

  inline StringDecorator &append(const CharT *s);

  template <typename InputIt> inline StringDecorator &append(InputIt first, InputIt last);

  template <typename T> inline StringDecorator &append(const T &t);

  template <typename T> inline StringDecorator &append(const T &t, size_type pos, size_type count = npos);

  StringDecorator operator+(const StringDecorator &rhs);

  StringDecorator operator+(const StringType &rhs);

  StringDecorator operator+(const CharT *rhs);

  StringDecorator operator+(CharT rhs);

  int compare(const StringDecorator &str) const noexcept;

  int compare(const StringType &str) const noexcept;

  int compare(size_type pos1, size_type count1, const StringDecorator &str) const;

  int compare(size_type pos1, size_type count1, const StringType &str) const;

  int compare(size_type pos1, size_type count1, const StringDecorator &str, size_type pos2, size_type count2 = npos) const;

  int compare(size_type pos1, size_type count1, const StringType &str, size_type pos2, size_type count2 = npos) const;

  int compare(const CharT *s) const;

  int compare(size_type pos1, size_type count1, const CharT *s) const;

  int compare(size_type pos1, size_type count1, const CharT *s, size_type count2) const;

  template <class T> int compare(const T &t) const noexcept;

  template <class T> int compare(size_type pos1, size_type count1, const T &t) const;

  template <class T> int compare(size_type pos1, size_type count1, const T &t, size_type pos2, size_type count2 = npos) const;

  StringDecorator &replace(size_type pos, size_type count, const StringDecorator &str);

  StringDecorator &replace(size_type pos, size_type count, const StringType &str);

  StringDecorator &replace(const_iterator first, const_iterator last, const StringDecorator &str);

  StringDecorator &replace(const_iterator first, const_iterator last, const StringType &str);

  StringDecorator &replace(size_type pos, size_type count, const StringDecorator &str, size_type pos2, size_type count2 = npos);

  StringDecorator &replace(size_type pos, size_type count, const StringType &str, size_type pos2, size_type count2 = npos);

  template <class InputIt> StringDecorator &replace(const_iterator first, const_iterator last, InputIt first2, InputIt last2);

  StringDecorator &replace(size_type pos, size_type count, const CharT *cstr, size_type count2);

  StringDecorator &replace(const_iterator first, const_iterator last, const CharT *cstr, size_type count2);

  StringDecorator &replace(size_type pos, size_type count, const CharT *cstr);

  StringDecorator &replace(const_iterator first, const_iterator last, const CharT *cstr);

  StringDecorator &replace(size_type pos, size_type count, size_type count2, CharT ch);

  StringDecorator &replace(const_iterator first, const_iterator last, size_type count2, CharT ch);

  StringDecorator &replace(const_iterator first, const_iterator last, std::initializer_list<CharT> ilist);

  template <class T> StringDecorator &replace(size_type pos, size_type count, const T &t);

  template <class T> StringDecorator &replace(const_iterator first, const_iterator last, const T &t);

  template <class T>
  StringDecorator &replace(size_type pos, size_type count, const T &t, size_type pos2, size_type count2 = npos);

  inline bool operator==(const StringDecorator &rhs) const;

  inline bool operator!=(const StringDecorator &rhs) const;

  inline bool operator<(const StringDecorator &rhs) const;

  inline bool operator>(const StringDecorator &rhs) const;

  inline bool operator<=(const StringDecorator &rhs) const;

  inline bool operator>=(const StringDecorator &rhs) const;

  inline bool operator==(const StringType &rhs) const;

  inline bool operator!=(const StringType &rhs) const;

  inline bool operator<(const StringType &rhs) const;

  inline bool operator>(const StringType &rhs) const;

  inline bool operator<=(const StringType &rhs) const;

  inline bool operator>=(const StringType &rhs) const;

  inline std::istream &operator>>(std::istream &is);

  template <template <class> typename ContainerOut = std::vector>
  inline ContainerOut<std::string> split(const StringType &delimiter);

  template <typename To> inline std::optional<To> to();

  inline bool starts_with(const StringType &str);

  inline bool ends_with(const StringType &str);

  inline StringDecorator toLower();

  inline StringDecorator toUpper();

  inline const StringType &string() const;

  template <typename... Args> constexpr inline StringDecorator format(Args &&... args);

  template <typename... Args> constexpr static inline StringDecorator Format(const StringType &str, Args &&... args);

  template <typename... Args> constexpr static inline StringDecorator Format(const StringDecorator &str, Args &&... args);

  inline const CharT *c_str() const { return _data.c_str(); }

private:
  StringType _data;
};

template <typename StringType> inline std::ostream &operator<<(std::ostream &os, const StringDecorator<StringType> &decorator);

template <typename StringType>
StringDecorator<StringType> operator+(const StringType &lhs, const StringDecorator<StringType> &rhs);

template <typename StringType> StringDecorator<StringType> operator+(const typename StringDecorator<StringType>::CharT *lhs,
                                                                     const StringDecorator<StringType> &rhs);

template <typename StringType>
StringDecorator<StringType> operator+(typename StringDecorator<StringType>::CharT lhs, const StringDecorator<StringType> &rhs);

// TODO: operators

#include "StringUtils.tpp"

#endif // UTILITIES_STRINGUTILS_H
