//
#include <zax.hpp>
#include <zax/unicode.hpp>
#include <cstring>

namespace zax::text {

namespace unicode_internal {
template <typename T> class Literal;
template <> class Literal<char> {
public:
  static constexpr std::string_view Empty = "\"\"";
  static constexpr std::string_view UnicodePrefix = "\\U";
};
template <> class Literal<wchar_t> {
public:
#if defined(_MSC_VER) || defined(_LIBCPP_VERSION)
  static constexpr std::wstring_view Empty = L"\"\"";
  static constexpr std::wstring_view UnicodePrefix = L"\\U";
#else
  // libstdc++ call wcslen is bad
  static constexpr std::wstring_view Empty{L"\"\"", sizeof("\"\"") - 1};
  static constexpr std::wstring_view UnicodePrefix = {L"\\U", sizeof("\\U") - 1};
#endif
};
template <> class Literal<char16_t> {
public:
  static constexpr std::u16string_view Empty = u"\"\"";
  static constexpr std::u16string_view UnicodePrefix = u"\\U";
};
} // namespace unicode_internal
/*
 * Index into the table below with the first byte of a UTF-8 sequence to
 * get the number of trailing bytes that are supposed to follow it.
 * Note that *legal* UTF-8 values can't have 4 or 5-bytes. The table is
 * left as-is for anyone who may want to do such conversion, which was
 * allowed in earlier algorithms.
 */
// clang-format off
static const char trailingbytesu8[256] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};
// clang-format on
constexpr const char32_t offsetfromu8[6] = {0x00000000UL, 0x00003080UL, 0x000E2080UL,
                                            0x03C82080UL, 0xFA082080UL, 0x82082080UL};

// char32_t - type for UTF-32 character representation, required to be large
// enough to represent any UTF-32 code unit (32 bits). It has the same size,
// signedness, and alignment as std::uint_least32_t, but is a distinct type.

inline constexpr bool IsSurrogate(char32_t rune) { return (rune >= 0xD800 && rune <= 0xDFFF); }

size_t char32tochar16(char32_t rune, char16_t *dest, size_t dlen) {
  if (dlen == 0 || dest == nullptr) {
    return 0;
  }
  if (rune <= 0xFFFF) {
    dest[0] = IsSurrogate(rune) ? 0xFFFD : static_cast<char16_t>(rune);
    return 1;
  }
  if (rune > 0x0010FFFF) {
    dest[0] = 0xFFFD;
    return 1;
  }
  if (dlen < 2) {
    return 0;
  }
  dest[0] = static_cast<char16_t>(0xD7C0 + (rune >> 10));
  dest[1] = static_cast<char16_t>(0xDC00 + (rune & 0x3FF));
  return 2;
}

static inline size_t char32tochar8_internal(char32_t rune, char *dest) {
  if (rune <= 0x7F) {
    dest[0] = static_cast<char>(rune);
    return 1;
  }
  if (rune <= 0x7FF) {
    dest[0] = static_cast<char>(0xC0 | ((rune >> 6) & 0x1F));
    dest[1] = static_cast<char>(0x80 | (rune & 0x3F));
    return 2;
  }
  if (rune <= 0xFFFF) {
    dest[0] = static_cast<char>(0xE0 | ((rune >> 12) & 0x0F));
    dest[1] = static_cast<char>(0x80 | ((rune >> 6) & 0x3F));
    dest[2] = static_cast<char>(0x80 | (rune & 0x3F));
    return 3;
  }
  if (rune <= 0x10FFFF) {
    dest[0] = static_cast<char>(0xF0 | ((rune >> 18) & 0x07));
    dest[1] = static_cast<char>(0x80 | ((rune >> 12) & 0x3F));
    dest[2] = static_cast<char>(0x80 | ((rune >> 6) & 0x3F));
    dest[3] = static_cast<char>(0x80 | (rune & 0x3F));
    return 4;
  }
  return 0;
}

size_t char32tochar8(char32_t rune, char *dest, size_t dlen) {
  constexpr const size_t kMaxEncodedUTF8Size = 4;
  if (dlen < kMaxEncodedUTF8Size) {
    return 0;
  }
  return char32tochar8_internal(rune, dest);
}

std::string u16tou8(const char16_t *u16, size_t len) {
  std::string s;
  s.reserve(len);
  auto it = u16;
  auto end = it + len;
  char buffer[8] = {0};
  while (it < end) {
    char32_t ch = *it++;
    if (ch >= 0xD800 && ch <= 0xDBFF) {
      if (it >= end) {
        return s;
      }
      char32_t ch2 = *it;
      if (ch2 < 0xDC00 || ch2 > 0xDFFF) {
        break;
      }
      ch = ((ch - 0xD800) << 10) + (ch2 - 0xDC00) + 0x10000U;
      ++it;
    }
    auto bw = char32tochar8_internal(ch, buffer);
    s.append(reinterpret_cast<const char *>(buffer), bw);
  }
  return s;
}

inline char32_t annex_u8(const uint8_t *it, int nb) {
  char32_t ch = 0;
  switch (nb) {
  case 5:
    ch += *it++;
    ch <<= 6; /* remember, illegal UTF-8 */
    [[fallthrough]];
  case 4:
    ch += *it++;
    ch <<= 6; /* remember, illegal UTF-8 */
    [[fallthrough]];
  case 3:
    ch += *it++;
    ch <<= 6;
    [[fallthrough]];
  case 2:
    ch += *it++;
    ch <<= 6;
    [[fallthrough]];
  case 1:
    ch += *it++;
    ch <<= 6;
    [[fallthrough]];
  case 0:
    ch += *it++;
  }
  ch -= offsetfromu8[nb];
  return ch;
}

template <typename T, typename Allocator>
bool u8tou16_internal(const uint8_t *s, size_t len,
                      std::basic_string<T, std::char_traits<T>, Allocator> &u16) {
  if (s == nullptr || len == 0) {
    return false;
  }
  container.reserve(len);
  auto it = reinterpret_cast<const unsigned char *>(s);
  auto end = it + len;
  while (it < end) {
    unsigned short nb = trailingbytesu8[*it];
    if (nb >= end - it) {
      return false;
    }
    // https://docs.microsoft.com/en-us/cpp/cpp/attributes?view=vs-2019
    auto ch = annex_u8(it, nb);
    it += nb + 1;
    if (ch <= 0xFFFF) {
      if (ch >= 0xD800 && ch <= 0xDBFF) {
        u16 += static_cast<T>(0xFFFD);
        continue;
      }
      u16 += static_cast<T>(ch);
      continue;
    }
    if (ch > 0x10FFFF) {
      u16 += static_cast<T>(0xFFFD);
      continue;
    }
    ch -= 0x10000U;
    u16 += static_cast<T>((ch >> 10) + 0xD800);
    u16 += static_cast<T>((ch & 0x3FF) + 0xDC00);
  }
  //
  return true;
}

std::u16string u8tou16(const char *u8, size_t len) {
  std::u16string s;
  if (!u8tou16_internal(reinterpret_cast<const uint8_t *>(u8), len, s)) {
    s.clear();
  }
  return s;
}

#ifdef _WIN32

inline constexpr size_t minlen(size_t a, size_t b) {
  if (a > b) {
    return a * 2;
  }
  return b * 2 + 1;
}

class CharBuffer {
public:
  CharBuffer(size_t bufsz = 1024) {
    data_ = reinterpret_cast<char *>(malloc(bufsz));
    memset(data_, 0, bufsz);
    cap_ = bufsz;
  }
  CharBuffer(const CharBuffer &) = delete;
  CharBuffer &operator=(const CharBuffer &) = delete;
  ~CharBuffer() {
    if (data_ != nullptr) {
      free(data_);
    }
  }
  size_t size() const { return len_; }
  char *detach() {
    auto p = data_;
    data_ = nullptr;
    cap_ = 0;
    return p;
  }
  bool append(const char *data, size_t len) {
    if (cap_ - len_ < len + 1) {
      auto ml = minlen(cap_, len);
      auto buf = reinterpret_cast<char *>(malloc(ml));
      if (buf == nullptr) {
        return false;
      }
      memcpy(buf, data_, len_);
      auto olddata = data_;
      data_ = buf;
      cap_ = ml;
      free(olddata);
    }
    memcpy(data_ + len_, data, len);
    len_ += len;
    data_[len_] = 0;
    return true;
  }

private:
  char *data_{nullptr};
  size_t cap_{0};
  size_t len_{0};
};

char *u16tou8(const wchar_t *wstr) {
  auto len = wcslen(wstr);
  CharBuffer cb(len / 2 * 3);
  auto it = wstr;
  auto end = it + len;
  char buffer[8] = {0};
  while (it < end) {
    char32_t ch = *it++;
    if (ch >= 0xD800 && ch <= 0xDBFF) {
      if (it >= end) {
        return cb.detach();
      }
      char32_t ch2 = *it;
      if (ch2 < 0xDC00 || ch2 > 0xDFFF) {
        break;
      }
      ch = ((ch - 0xD800) << 10) + (ch2 - 0xDC00) + 0x10000U;
      ++it;
    }
    auto bw = char32tochar8_internal(ch, buffer);
    cb.append(reinterpret_cast<const char *>(buffer), bw);
  }
  return cb.detach();
}

std::wstring u8towc(const char *u8, size_t len) {
  std::wstring s;
  if (!u8tou16_internal(reinterpret_cast<const uint8_t *>(u8), len, s)) {
    s.clear();
  }
  return s;
}
#endif
} // namespace zax::text