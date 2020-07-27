#ifndef ZAX_STRINGCAT_HPP
#define ZAX_STRINGCAT_HPP
#include <string>
#include <string_view>
#include <charconv>
#include "unicode.hpp"

namespace zax {
//
class AlphaNum {
  static constexpr const int suggest_size = 32;

public:
  AlphaNum(int x) {
    auto ret = std::to_chars(digits_, digits_ + suggest_size, x);
    piece_ = std::string_view{digits_, ret.ptr - digits_};
  }
  AlphaNum(unsigned int x) {
    auto ret = std::to_chars(digits_, digits_ + suggest_size, x);
    piece_ = std::string_view{digits_, ret.ptr - digits_};
  }
  AlphaNum(long x) {
    auto ret = std::to_chars(digits_, digits_ + suggest_size, x);
    piece_ = std::string_view{digits_, ret.ptr - digits_};
  }
  AlphaNum(unsigned long x) {
    auto ret = std::to_chars(digits_, digits_ + suggest_size, x);
    piece_ = std::string_view{digits_, ret.ptr - digits_};
  }
  AlphaNum(long long x) {
    auto ret = std::to_chars(digits_, digits_ + suggest_size, x);
    piece_ = std::string_view{digits_, ret.ptr - digits_};
  }
  AlphaNum(unsigned long long x) {
    auto ret = std::to_chars(digits_, digits_ + suggest_size, x);
    piece_ = std::string_view{digits_, ret.ptr - digits_};
  }
  AlphaNum(char32_t rune) {
    // convert utf-8
    auto n = zax::text::RuneEncode(rune, digits_, suggest_size);
    piece_ = std::string_view{digits_, n};
  }
  AlphaNum(const char *c_str) : piece_(c_str) {}
  AlphaNum(std::string_view pc) : piece_(pc) {}

  AlphaNum(char c) = delete;

  AlphaNum(const AlphaNum &) = delete;
  AlphaNum &operator=(const AlphaNum &) = delete;

  std::string_view::size_type size() const { return piece_.size(); }
  const char *data() const { return piece_.data(); }
  std::string_view Piece() const { return piece_; }

private:
  std::string_view piece_;
  char digits_[suggest_size];
};

} // namespace zax

#endif