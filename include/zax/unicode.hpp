//
#ifndef ZAX_UNICODE_HPP
#define ZAX_UNICODE_HPP
#include <string_view>

namespace zax::text {
// char32_t - type for UTF-32 character representation, required to be large
// enough to represent any UTF-32 code unit (32 bits). It has the same size,
// signedness, and alignment as std::uint_least32_t, but is a distinct type.

inline constexpr bool IsSurrogate(char32_t rune) { return (rune >= 0xD800 && rune <= 0xDFFF); }
size_t RuneEncode(char32_t rune, char *dest, size_t dlen);
// ToNarrow convert UTF-16 to UTF-8
std::string ToNarrow(const char16_t *u16, size_t len);
inline std::string ToNarrow(std::u16string_view u16) { return ToNarrow(u16.data(), u16.size()); }
std::u16string ToWide(const char *u8, size_t len);
inline std::u16string ToWide(std::string_view u8) { return ToWide(u8.data(), u8.size()); }

#ifdef _WIN32
char *ToNarrow(const wchar_t *wstr);
inline std::string ToNarrow(const wchar_t *u16, size_t len) {
  return ToNarrow(reinterpret_cast<const char16_t *>(u16), len);
}
inline std::string ToNarrow(std::wstring_view u16) {
  return ToNarrow(reinterpret_cast<const char16_t *>(u16.data()), u16.size());
}
std::wstring Narrow2Wide(const char *u8, size_t len);
inline std::wstring Narrow2Wide(std::string_view str) { return Narrow2Wide(str.data(), str.size()); }

#endif

} // namespace zax::text

#endif