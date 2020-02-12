
template <template <class> typename ContainerOut, typename StringType>
ContainerOut<StringType> StringUtils::split(const StringType &str, const StringType &delimiter) {
  ContainerOut<StringType> result;
  size_t first = 0;
  while (first < str.size()) {
    const auto second = str.find_first_of(delimiter, first);
    if (first != second) {
      result.emplace_back(str.substr(first, second - first));
    }
    if (second == StringType::npos) {
      break;
    }
    first = second + 1;
  }
  return result;
}

template <typename To, typename StringType> std::optional<To> StringUtils::to(const StringType &str) {
  try {
    if constexpr (std::is_same_v<To, int>) {
      return std::stoi(str);
    } else if constexpr (std::is_same_v<To, float>) {
      return std::stof(str);
    } else if constexpr (std::is_same_v<To, double>) {
      return std::stod(str);
    } else if constexpr (std::is_same_v<To, bool>) {
      const auto lower = StringUtils::toLower(str);
      return lower != "false" && lower != "0";
    } else {
      return static_cast<To>(str);
    }
  } catch (...) {
    return std::nullopt;
  }
}

template <typename StringType> template <typename... Args> StringDecorator<StringType>::StringDecorator(const Args &... args)
    : _data(std::forward<const Args &>(args)...) {}

template <typename StringType> template <typename T>
StringDecorator<StringType> &StringDecorator<StringType>::operator=(const T &other) {
  if constexpr (std::is_same_v<T, StringDecorator>) {
    _data = other.data;
  } else {
    _data = other;
  }
  return *this;
}

template <typename StringType> template <typename T>
StringDecorator<StringType> &StringDecorator<StringType>::operator=(T &&other) {
  if constexpr (std::is_same_v<T, StringDecorator>) {
    _data = std::move(other.data);
  } else {
    _data = std::forward<T>(other);
  }
  return *this;
}

template <typename StringType> template <typename T, typename> StringDecorator<StringType>::operator T() const { return _data; }

template <typename StringType> typename StringDecorator<StringType>::reference StringDecorator<StringType>::at(size_type pos) {
  return _data.at(pos);
}

template <typename StringType>
typename StringDecorator<StringType>::const_reference StringDecorator<StringType>::at(size_type pos) const {
  return _data.at(pos);
}

template <typename StringType>
typename StringDecorator<StringType>::reference StringDecorator<StringType>::operator[](size_type pos) {
  return _data[pos];
}

template <typename StringType>
typename StringDecorator<StringType>::const_reference StringDecorator<StringType>::operator[](size_type pos) const {
  return _data[pos];
}

template <typename StringType> typename StringDecorator<StringType>::CharT &StringDecorator<StringType>::front() {
  return _data.front();
}

template <typename StringType> const typename StringDecorator<StringType>::CharT &StringDecorator<StringType>::front() const {
  return _data.front();
}

template <typename StringType> typename StringDecorator<StringType>::CharT &StringDecorator<StringType>::back() {
  return _data.back();
}

template <typename StringType> const typename StringDecorator<StringType>::CharT &StringDecorator<StringType>::back() const {
  return _data.back();
}

template <typename StringType> typename StringDecorator<StringType>::CharT *StringDecorator<StringType>::data() {
  return _data.data();
}

template <typename StringType> const typename StringDecorator<StringType>::CharT *StringDecorator<StringType>::data() const {
  return _data.data();
}

template <typename StringType> typename StringDecorator<StringType>::iterator StringDecorator<StringType>::begin() noexcept {
  return _data.begin();
}

template <typename StringType> typename StringDecorator<StringType>::const_iterator StringDecorator<StringType>::begin() const
    noexcept {
  return _data.begin();
}

template <typename StringType> typename StringDecorator<StringType>::const_iterator StringDecorator<StringType>::cbegin() const
    noexcept {
  return _data.cbegin();
}

template <typename StringType> typename StringDecorator<StringType>::iterator StringDecorator<StringType>::end() noexcept {
  return _data.end();
}

template <typename StringType> typename StringDecorator<StringType>::const_iterator StringDecorator<StringType>::end() const
    noexcept {
  return _data.end();
}

template <typename StringType> typename StringDecorator<StringType>::const_iterator StringDecorator<StringType>::cend() const
    noexcept {
  return _data.cend();
}

template <typename StringType>
typename StringDecorator<StringType>::reverse_iterator StringDecorator<StringType>::rbegin() noexcept {
  return _data.rbegin();
}

template <typename StringType>
typename StringDecorator<StringType>::const_reverse_iterator StringDecorator<StringType>::rbegin() const noexcept {
  return _data.rbegin();
}

template <typename StringType>
typename StringDecorator<StringType>::const_reverse_iterator StringDecorator<StringType>::crbegin() const noexcept {
  return _data.crbegin();
}

template <typename StringType>
typename StringDecorator<StringType>::reverse_iterator StringDecorator<StringType>::rend() noexcept {
  return _data.rend();
}

template <typename StringType>
typename StringDecorator<StringType>::const_reverse_iterator StringDecorator<StringType>::rend() const noexcept {
  return _data.rend();
}

template <typename StringType>
typename StringDecorator<StringType>::const_reverse_iterator StringDecorator<StringType>::crend() const noexcept {
  return _data.crend();
}

template <typename StringType> bool StringDecorator<StringType>::empty() const noexcept { return _data.empty(); }

template <typename StringType> typename StringDecorator<StringType>::size_type StringDecorator<StringType>::size() const
    noexcept {
  return _data.size();
}

template <typename StringType> typename StringDecorator<StringType>::size_type StringDecorator<StringType>::length() const
    noexcept {
  return _data.length();
}

template <typename StringType> typename StringDecorator<StringType>::size_type StringDecorator<StringType>::max_size() const
    noexcept {
  return _data.max_size();
}

template <typename StringType> void StringDecorator<StringType>::reserve(size_type new_cap) { _data.reserve(new_cap); }

template <typename StringType> typename StringDecorator<StringType>::size_type StringDecorator<StringType>::capacity() const
    noexcept {
  return _data.capacity();
}

template <typename StringType> void StringDecorator<StringType>::shrink_to_fit() { _data.shrink_to_fit(); }

template <typename StringType> void StringDecorator<StringType>::clear() noexcept { _data.clear(); }

template <typename StringType> void StringDecorator<StringType>::swap(StringDecorator &other) noexcept { _data.swap(other); }

template <typename StringType> void StringDecorator<StringType>::resize(size_type count) { _data.resize(count); }

template <typename StringType> void StringDecorator<StringType>::resize(size_type count, CharT ch) { _data.resize(count, ch); }

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::copy(CharT *dest, size_type count, size_type pos) const {
  return _data.copy(dest, count, pos);
}

template <typename StringType> StringDecorator<StringType> StringDecorator<StringType>::substr(size_type pos, size_type count) {
  return StringDecorator{_data.substr(std::forward<size_type>(pos), std::forward<size_type>(count))};
}

template <typename StringType> bool StringDecorator<StringType>::operator==(const StringDecorator &rhs) const {
  return _data == rhs._data;
}

template <typename StringType> bool StringDecorator<StringType>::operator!=(const StringDecorator &rhs) const {
  return !(rhs == *this);
}

template <typename StringType> bool StringDecorator<StringType>::operator<(const StringDecorator &rhs) const {
  return _data < rhs._data;
}

template <typename StringType> bool StringDecorator<StringType>::operator>(const StringDecorator &rhs) const {
  return rhs < *this;
}

template <typename StringType> bool StringDecorator<StringType>::operator<=(const StringDecorator &rhs) const {
  return !(rhs < *this);
}

template <typename StringType> bool StringDecorator<StringType>::operator>=(const StringDecorator &rhs) const {
  return !(*this < rhs);
}

template <typename StringType> bool StringDecorator<StringType>::operator==(const StringType &rhs) const { return _data == rhs; }

template <typename StringType> bool StringDecorator<StringType>::operator!=(const StringType &rhs) const {
  return !(*this == rhs);
}

template <typename StringType> bool StringDecorator<StringType>::operator<(const StringType &rhs) const { return _data < rhs; }

template <typename StringType> bool StringDecorator<StringType>::operator>(const StringType &rhs) const { return *this > rhs; }

template <typename StringType> bool StringDecorator<StringType>::operator<=(const StringType &rhs) const {
  return !(*this > rhs);
}

template <typename StringType> bool StringDecorator<StringType>::operator>=(const StringType &rhs) const {
  return !(*this < rhs);
}

template <typename StringType> std::ostream &operator<<(std::ostream &os, const StringDecorator<StringType> &decorator) {
  os << decorator.string();
  return os;
}

template <typename StringType>
StringDecorator<StringType> operator+(const StringType &lhs, const StringDecorator<StringType> &rhs) {
  return lhs + rhs._data;
}

template <typename StringType> StringDecorator<StringType> operator+(const typename StringDecorator<StringType>::CharT *lhs,
                                                                     const StringDecorator<StringType> &rhs) {
  return lhs + rhs._data;
}

template <typename StringType>
StringDecorator<StringType> operator+(typename StringDecorator<StringType>::CharT lhs, const StringDecorator<StringType> &rhs) {
  return lhs + rhs._data;
}

template <typename StringType> std::istream &StringDecorator<StringType>::operator>>(std::istream &is) {
  is >> _data;
  return is;
}

template <typename StringType> template <typename T>
StringDecorator<StringType> &StringDecorator<StringType>::operator+=(const T &t) {
  _data += t;
  return *this;
}

template <typename StringType> void StringDecorator<StringType>::pop_back() { _data.pop_back(); }

template <typename StringType> void StringDecorator<StringType>::push_back(CharT ch) { _data.push_back(ch); }

template <typename StringType>
typename StringDecorator<StringType>::iterator StringDecorator<StringType>::erase(const_iterator position) {
  return _data.erase(position);
}

template <typename StringType>
typename StringDecorator<StringType>::iterator StringDecorator<StringType>::erase(const_iterator first, const_iterator last) {
  return _data.erase(first, last);
}

template <typename StringType>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::find(const StringDecorator &str, size_type pos) const
    noexcept {
  return _data.find(str._data, pos);
}

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::find(const CharT *s, size_type pos, size_type count) const {
  return _data.find(s, pos, count);
}

template <typename StringType>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::find(const CharT *s, size_type pos) const {
  return _data.find(s, pos);
}

template <typename StringType>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::find(CharT ch, size_type pos) const noexcept {
  return _data.find(ch, pos);
}

template <typename StringType> template <typename T>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::find(const T &t, size_type pos) const noexcept {
  return _data.find(t, pos);
}

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::rfind(const StringDecorator &str, size_type pos) const noexcept {
  return _data.rfind(str, pos);
}

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::rfind(const CharT *s, size_type pos, size_type count) const {
  return _data.rfind(s, pos, count);
}

template <typename StringType>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::rfind(const CharT *s, size_type pos) const {
  return _data.rfind(s, pos);
}

template <typename StringType>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::rfind(CharT ch, size_type pos) const noexcept {
  return _data.rfind(ch, pos);
}

template <typename StringType> template <typename T>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::rfind(const T &t, size_type pos) const noexcept {
  return _data.rfind(t, pos);
}

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::find_first_of(const StringDecorator &str, size_type pos) const noexcept {
  return _data.find_first_of(str._data, pos);
}

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::find_first_of(const CharT *s, size_type pos, size_type count) const {
  return _data.find_first_of(s, pos, count);
}

template <typename StringType>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::find_first_of(const CharT *s, size_type pos) const {
  return _data.find_first_of(s, pos);
}

template <typename StringType>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::find_first_of(CharT ch, size_type pos) const
    noexcept {
  return _data.find_first_of(ch, pos);
}

template <typename StringType> template <class T>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::find_first_of(const T &t, size_type pos) const
    noexcept {
  return _data.find_first_of(t, pos);
}

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::find_first_not_of(const StringDecorator &str, size_type pos) const noexcept {
  return _data.find_first_not_of(str._data, pos);
}

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::find_first_not_of(const CharT *s, size_type pos, size_type count) const {
  return _data.find_first_not_of(s, pos, count);
}

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::find_first_not_of(const CharT *s, size_type pos) const {
  return _data.find_first_not_of(s, pos);
}

template <typename StringType>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::find_first_not_of(CharT ch, size_type pos) const
    noexcept {
  return _data.find_first_not_of(ch, pos);
}

template <typename StringType> template <class T>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::find_first_not_of(const T &t, size_type pos) const
    noexcept {
  return _data.find_first_not_of(t, pos);
}

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::find_last_of(const StringDecorator &str, size_type pos) const noexcept {
  return _data.find_last_of(str._data, pos);
}

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::find_last_of(const CharT *s, size_type pos, size_type count) const {
  return _data.find_last_of(s, pos, count);
}

template <typename StringType>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::find_last_of(const CharT *s, size_type pos) const {
  return _data.find_last_of(s, pos);
}

template <typename StringType>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::find_last_of(CharT ch, size_type pos) const
    noexcept {
  return _data.find_last_of(ch, pos);
}

template <typename StringType> template <class T>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::find_last_of(const T &t, size_type pos) const
    noexcept {
  return _data.find_last_of(t, pos);
}

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::find_last_not_of(const StringDecorator &str, size_type pos) const noexcept {
  return _data.find_last_not_of(str._data, pos);
}

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::find_last_not_of(const CharT *s, size_type pos, size_type count) const {
  return _data.find_last_not_of(s, pos, count);
}

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::find_last_not_of(const CharT *s, size_type pos) const {
  return _data.find_last_not_of(s, pos);
}

template <typename StringType>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::find_last_not_of(CharT ch, size_type pos) const
    noexcept {
  return _data.find_last_not_of(ch, pos);
}

template <typename StringType> template <class T>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::find_last_not_of(const T &t, size_type pos) const
    noexcept {
  return _data.find_last_not_of(t, pos);
}

template <typename StringType> StringDecorator<StringType> &StringDecorator<StringType>::insert(size_type index, const CharT *s) {
  _data.insert(index, s);
  return *this;
}

template <typename StringType>
StringDecorator<StringType> &StringDecorator<StringType>::insert(size_type index, const CharT *s, size_type count) {
  _data.insert(index, s, count);
  return *this;
}

template <typename StringType>
StringDecorator<StringType> &StringDecorator<StringType>::insert(size_type index, const StringDecorator &str) {
  _data.insert(index, str);
  return *this;
}

template <typename StringType> StringDecorator<StringType> &
StringDecorator<StringType>::insert(size_type index, const StringDecorator &str, size_type index_str, size_type count) {
  _data.insert(index, str, index_str, count);
  return *this;
}

template <typename StringType>
typename StringDecorator<StringType>::iterator StringDecorator<StringType>::insert(const_iterator pos, CharT ch) {
  return _data.insert(pos, ch);
}

template <typename StringType> template <typename InputIt> typename StringDecorator<StringType>::iterator
StringDecorator<StringType>::insert(const_iterator pos, InputIt first, InputIt last) {
  return _data.insert(pos, first, last);
}

template <typename StringType> typename StringDecorator<StringType>::iterator
StringDecorator<StringType>::insert(const_iterator pos, std::initializer_list<CharT> ilist) {
  return _data.insert(pos, ilist);
}

template <typename StringType> template <typename T>
StringDecorator<StringType> &StringDecorator<StringType>::insert(size_type pos, const T &t) {
  _data.insert(pos, t);
  return *this;
}

template <typename StringType> template <typename T> StringDecorator<StringType> &
StringDecorator<StringType>::insert(size_type index, const T &t, size_type index_str, size_type count) {
  _data.insert(index, t, index_str, count);
  return *this;
}

template <typename StringType> StringDecorator<StringType> &StringDecorator<StringType>::append(const StringDecorator &str) {
  _data.append(str._data);
  return *this;
}

template <typename StringType>
StringDecorator<StringType> &StringDecorator<StringType>::append(const StringDecorator &str, size_type pos, size_type count) {
  _data.append(str._data, pos, count);
  return *this;
}

template <typename StringType> StringDecorator<StringType> &StringDecorator<StringType>::append(const CharT *s, size_type count) {
  _data.append(s, count);
  return *this;
}

template <typename StringType> StringDecorator<StringType> &StringDecorator<StringType>::append(const CharT *s) {
  _data.append(s);
  return *this;
}

template <typename StringType> template <typename InputIt>
StringDecorator<StringType> &StringDecorator<StringType>::append(InputIt first, InputIt last) {
  _data.append(first, last);
  return *this;
}

template <typename StringType> template <typename T>
StringDecorator<StringType> &StringDecorator<StringType>::append(const T &t) {
  _data.append(t);
  return *this;
}

template <typename StringType> template <typename T>
StringDecorator<StringType> &StringDecorator<StringType>::append(const T &t, size_type pos, size_type count) {
  _data.append(t, pos, count);
  return *this;
}

template <typename StringType> StringDecorator<StringType> StringDecorator<StringType>::operator+(const StringDecorator &rhs) {
  return _data + rhs._data;
}

template <typename StringType> StringDecorator<StringType> StringDecorator<StringType>::operator+(const StringType &rhs) {
  return _data + rhs;
}

template <typename StringType> StringDecorator<StringType> StringDecorator<StringType>::operator+(const CharT *rhs) {
  return _data + rhs;
}

template <typename StringType> StringDecorator<StringType> StringDecorator<StringType>::operator+(const CharT rhs) {
  return _data + rhs;
}

template <typename StringType> int StringDecorator<StringType>::compare(const StringDecorator &str) const noexcept {
  return _data.compare(str._data);
}

template <typename StringType>
int StringDecorator<StringType>::compare(size_type pos1, size_type count1, const StringDecorator &str) const {
  return _data.compare(pos1, count1, str._data);
}

template <typename StringType> int StringDecorator<StringType>::compare(size_type pos1, size_type count1,
                                                                        const StringDecorator &str, size_type pos2,
                                                                        size_type count2) const {
  return _data.compare(pos1, count1, str._data, pos2, count2);
}

template <typename StringType> int StringDecorator<StringType>::compare(const CharT *s) const { return _data.compare(s); }

template <typename StringType> int StringDecorator<StringType>::compare(size_type pos1, size_type count1, const CharT *s) const {
  return _data.compare(pos1, count1, s);
}

template <typename StringType>
int StringDecorator<StringType>::compare(size_type pos1, size_type count1, const CharT *s, size_type count2) const {
  return _data.compare(pos1, count1, s, count2);
}

template <typename StringType> template <class T> int StringDecorator<StringType>::compare(const T &t) const noexcept {
  return _data.compare(t);
}

template <typename StringType> template <class T>
int StringDecorator<StringType>::compare(size_type pos1, size_type count1, const T &t) const {
  return _data.compare(pos1, count1, t);
}

template <typename StringType> template <class T>
int StringDecorator<StringType>::compare(size_type pos1, size_type count1, const T &t, size_type pos2, size_type count2) const {
  return _data.compare(pos1, count1, t, pos2, count2);
}

template <typename StringType> StringDecorator<StringType> &
StringDecorator<StringType>::replace(const_iterator first, const_iterator last, const StringDecorator &str) {
  _data.replace(first, last, str._data);
  return *this;
}

template <typename StringType>
StringDecorator<StringType> &StringDecorator<StringType>::replace(size_type pos, size_type count, const StringDecorator &str,
                                                                  size_type pos2, size_type count2) {
  _data.replace(pos, count, str._data, pos2, count2);
  return *this;
}

template <typename StringType> StringDecorator<StringType> &
StringDecorator<StringType>::replace(const_iterator first, const_iterator last, const CharT *cstr, size_type count2) {
  _data.replace(first, last, cstr, count2);
  return *this;
}

template <typename StringType>
StringDecorator<StringType> &StringDecorator<StringType>::replace(const_iterator first, const_iterator last, const CharT *cstr) {
  _data.replace(first, last, cstr);
  return *this;
}

template <typename StringType> StringDecorator<StringType> &
StringDecorator<StringType>::replace(const_iterator first, const_iterator last, size_type count2, CharT ch) {
  _data.replace(first, last, count2, ch);
  return *this;
}

template <typename StringType> StringDecorator<StringType> &
StringDecorator<StringType>::replace(const_iterator first, const_iterator last, std::initializer_list<CharT> ilist) {
  _data.replace(first, last, ilist);
  return *this;
}

template <typename StringType> template <class T>
StringDecorator<StringType> &StringDecorator<StringType>::replace(const_iterator first, const_iterator last, const T &t) {
  _data.replace(first, last, t);
  return *this;
}

template <typename StringType> template <class T> StringDecorator<StringType> &
StringDecorator<StringType>::replace(size_type pos, size_type count, const T &t, size_type pos2, size_type count2) {
  _data.replace(pos, count, t, pos2, count2);
  return *this;
}

template <typename StringType> StringDecorator<StringType> StringDecorator<StringType>::Random(size_type length) {
  return StringDecorator(StringUtils::getRandomString(length));
}

template <typename StringType> StringDecorator<StringType> StringDecorator<StringType>::toLower() {
  return StringDecorator(StringUtils::toLower(_data));
}

template <typename StringType> StringDecorator<StringType> StringDecorator<StringType>::toUpper() {
  return StringDecorator(StringUtils::toUpper(_data));
}

template <typename StringType> template <template <class> typename ContainerOut>
ContainerOut<std::string> StringDecorator<StringType>::split(const StringType &delimiter) {
  return StringUtils::split(_data, delimiter);
}

template <typename StringType> template <typename To> std::optional<To> StringDecorator<StringType>::to() {
  return StringUtils::to<To>(_data);
}

template <typename StringType> bool StringDecorator<StringType>::starts_with(const StringType &str) {
  return StringUtils::startsWith(_data, str);
}

template <typename StringType> bool StringDecorator<StringType>::ends_with(const StringType &str) {
  return StringUtils::endsWith(_data, str);
}

template <typename StringType> const StringType &StringDecorator<StringType>::string() const { return _data; }

template <typename StringType> template <typename... Args>
constexpr StringDecorator<StringType> StringDecorator<StringType>::format(Args &&... args) {
  return StringDecorator{fmt::format(_data, args...)};
}

template <typename StringType> template <typename... Args>
constexpr StringDecorator<StringType> StringDecorator<StringType>::Format(const StringType &str, Args &&... args) {
  return StringDecorator{fmt::format(str, args...)};
}

template <typename StringType> template <typename... Args>
constexpr StringDecorator<StringType> StringDecorator<StringType>::Format(const StringDecorator &str, Args &&... args) {
  return StringDecorator{fmt::format(str._data, args...)};
}

template <typename StringType> template <class T>
StringDecorator<StringType> &StringDecorator<StringType>::replace(size_type pos, size_type count, const T &t) {
  _data.replace(pos, count, t);
  return *this;
}

template <typename StringType>
StringDecorator<StringType> &StringDecorator<StringType>::replace(size_type pos, size_type count, size_type count2, CharT ch) {
  _data.replace(pos, count, count2, ch);
  return *this;
}

template <typename StringType>
StringDecorator<StringType> &StringDecorator<StringType>::replace(size_type pos, size_type count, const CharT *cstr) {
  _data.replace(pos, count, cstr, cstr);
  return *this;
}

template <typename StringType> StringDecorator<StringType> &
StringDecorator<StringType>::replace(size_type pos, size_type count, const CharT *cstr, size_type count2) {
  _data.replace(pos, count, cstr, count2);
  return *this;
}

template <typename StringType> template <class InputIt> StringDecorator<StringType> &
StringDecorator<StringType>::replace(const_iterator first, const_iterator last, InputIt first2, InputIt last2) {
  _data.replace(first, last, first2, last2);
  return *this;
}

template <typename StringType> StringDecorator<StringType> &
StringDecorator<StringType>::replace(size_type pos, size_type count, const StringType &str, size_type pos2, size_type count2) {
  _data.replace(pos, count, str, pos2, count2);
  return *this;
}

template <typename StringType> StringDecorator<StringType> &
StringDecorator<StringType>::replace(const_iterator first, const_iterator last, const StringType &str) {
  _data.replace(first, last, str);
  return *this;
}

template <typename StringType>
StringDecorator<StringType> &StringDecorator<StringType>::replace(size_type pos, size_type count, const StringType &str) {
  _data.replace(pos, count, str);
  return *this;
}

template <typename StringType>
StringDecorator<StringType> &StringDecorator<StringType>::replace(size_type pos, size_type count, const StringDecorator &str) {
  _data.replace(pos, count, str._data);
  return *this;
}

template <typename StringType> int StringDecorator<StringType>::compare(size_type pos1, size_type count1, const StringType &str,
                                                                        size_type pos2, size_type count2) const {
  return _data.compare(pos1, count1, str, pos2, count2);
}

template <typename StringType>
int StringDecorator<StringType>::compare(size_type pos1, size_type count1, const StringType &str) const {
  return _data.compare(pos1, count1, str);
}

template <typename StringType> int StringDecorator<StringType>::compare(const StringType &str) const noexcept {
  return _data.compare(str);
}

template <typename StringType>
StringDecorator<StringType> &StringDecorator<StringType>::append(const StringType &str, size_type pos, size_type count) {
  _data.append(str, pos, count);
  return *this;
}

template <typename StringType> StringDecorator<StringType> &StringDecorator<StringType>::append(const StringType &str) {
  _data.append(str);
  return *this;
}

template <typename StringType> StringDecorator<StringType> &StringDecorator<StringType>::append(size_type count, CharT ch) {
  _data.append(count, ch);
  return *this;
}

template <typename StringType> typename StringDecorator<StringType>::iterator
StringDecorator<StringType>::insert(const_iterator pos, size_type count, CharT ch) {
  return _data.insert(pos, count, ch);
}

template <typename StringType> StringDecorator<StringType> &
StringDecorator<StringType>::insert(size_type index, const StringType &str, size_type index_str, size_type count) {
  _data.insert(index, str, count);
  return *this;
}

template <typename StringType>
StringDecorator<StringType> &StringDecorator<StringType>::insert(size_type index, const StringType &str) {
  _data.insert(index, str);
  return *this;
}

template <typename StringType>
StringDecorator<StringType> &StringDecorator<StringType>::insert(size_type index, size_type count, CharT ch) {
  _data.insert(index, count, ch);
  return *this;
}

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::find_last_not_of(const StringType &str, size_type pos) const noexcept {
  return _data.find_last_not_of(str, pos);
}

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::find_last_of(const StringType &str, size_type pos) const noexcept {
  return _data.find_last_of(str, pos);
}

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::find_first_not_of(const StringType &str, size_type pos) const noexcept {
  return _data.find_first_not_of(str, pos);
}

template <typename StringType> typename StringDecorator<StringType>::size_type
StringDecorator<StringType>::find_first_of(const StringType &str, size_type pos) const noexcept {
  return _data.find_first_of(str, pos);
}

template <typename StringType>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::rfind(const StringType &str, size_type pos) const
    noexcept {
  return _data.rfind(str, pos);
}

template <typename StringType>
typename StringDecorator<StringType>::size_type StringDecorator<StringType>::find(const StringType &str, size_type pos) const
    noexcept {
  return _data.find(str, pos);
}

template <typename StringType> StringDecorator<StringType> &StringDecorator<StringType>::erase(size_type index, size_type count) {
  _data.erase(index, count);
  return *this;
}
