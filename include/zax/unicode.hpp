//
#ifndef ZAX_UNICODE_HPP
#define ZAX_UNICODE_HPP
#include <string_view>

namespace zax::text {
// u16tou8 convert UTF-16 to UTF-8
std::string u16tou8(const char16_t *u16, size_t len);
inline std::string u16tou8(std::u16string_view u16) { return u16tou8(u16.data(), u16.size()); }
std::u16string u8tou16(const char *u8, size_t len);
inline std::u16string u8tou16(std::string_view u8) { return u8tou16(u8.data(), u8.size()); }

#ifdef _WIN32
char *u16tou8(const wchar_t *wstr);
inline std::string u16tou8(const wchar_t *u16, size_t len) {
  return u16tou8(reinterpret_cast<const char16_t *>(u16), len);
}
inline std::string u16tou8(std::wstring_view u16) {
  return u16tou8(reinterpret_cast<const char16_t *>(u16.data()), u16.size());
}
std::wstring u8towc(const char *u8, size_t len);
inline std::wstring u8towc(std::string_view str) { return u8towc(str.data(), str.size()); }

#endif

} // namespace zax::text

#endif