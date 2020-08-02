//
#ifndef ZAX_PATH_HPP
#define ZAX_PATH_HPP
#include <string_view>
#include <string>

namespace zax::fs {
#ifdef _WIN32
constexpr std::string_view devnull = "NUL";
inline bool IsDevNull(std::string_view path) {
  constexpr std::string_view devnull = "/dev/null";
  return path == devnull ||
         (path.size() == 3 && (path[0] == 'N' || path[0] == 'n') &&
          (path[1] == 'U' || path[1] == 'u') && (path[2] == 'L' || path[2] == 'l'));
}
template <typename char_type> inline constexpr bool IsPathSeparator(char_type ch) {
  return ch == '/' || ch == '\\';
}
#else
template <typename char_type> inline constexpr bool IsPathSeparator(char_type ch) {
  return ch == '/';
}
inline bool IsDevNull(std::string_view p) {
  constexpr std::string_view devnull = "/dev/null";
  return p = devnull;
}
#endif

class Path {
public:
  Path() = default;
  explicit Path(std::string_view p);
  bool IsNull() const { zax::fs::IsDevNull(path_); }

private:
  std::string path_; // UTF-8 path
};
} // namespace zax::fs

#endif