// TOML Tom's Obvious Minimal Language
#ifndef ZAX_TOML_HPP
#define ZAX_TOML_HPP
#include <string_view>
#include <string>
#include <charconv>
#include <version>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cfloat>
#include <climits>
#include <optional>
#include <unordered_map>
// https://toml.io/en/
// https://toml.io/en/v1.0.0-rc.1
// https://github.com/toml-lang/toml/wiki

namespace toml {
struct string_hash {
  using is_transparent = void; // <-- It does not work without this trait
  size_t operator()(std::string_view txt) const { return std::hash<std::string_view>{}(txt); }
  size_t operator()(const std::string &txt) const { return std::hash<std::string>{}(txt); }
};
struct string_compare {
  using is_transparent = void; // Pred to use
  bool operator()(const std::string &key, std::string_view txt) const { return key == txt; }
  bool operator()(std::string_view key, std::string_view txt) const { return key == txt; }
};
// heterogeneous lookup
template <typename T>
using string_map = std::unordered_map<std::string, T, string_hash, string_compare>;

template <typename T> using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename T, typename... U>
inline constexpr bool is_one_of = (false || ... || std::is_same_v<T, U>);

template <typename T>
inline constexpr bool is_cvref =
    std::is_reference_v<T> || std::is_const_v<T> || std::is_volatile_v<T>;

template <typename T>
inline constexpr bool is_wide_string =
    is_one_of<std::decay_t<T>, const wchar_t *, wchar_t *, std::wstring_view, std::wstring>;

template <typename T> inline constexpr bool dependent_false = false;
// toml table
class Table {};
} // namespace toml

#endif