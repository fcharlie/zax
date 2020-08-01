//
#ifndef ZAX_FS_HPP
#define ZAX_FS_HPP
#ifdef _WIN32
#include <windows.h>
#endif
#include "zax.hpp"

namespace zax::fs {
#ifdef _WIN32
using native_handle = HANDLE;
#define closedfd INVALID_HANDLE_VALUE
#else
using native_handle = int;
constexpr auto closedfd = -1;
#endif

class File {
public:
  File() = default;
  File(const File &) = delete;
  File &operator=(const File &) = delete;
  ~File();
  ssize_t Write(const uint8_t *data, size_t len);
  ssize_t Read(uint8_t *buffer, size_t bufsz);
  int Close();

private:
  native_handle fd{closedfd};
};
} // namespace zax::fs

#endif