///
#ifndef ZAX_WIN_FILE_HPP
#define ZAX_WIN_FILE_HPP
#ifdef _WIN32
#include <windows.h>
#endif
#include <string_view>
#include <string>
#include <cstdint>

namespace zax::os::win {
using native_handle = decltype(INVALID_HANDLE_VALUE);
class FD {
public:
private:
};
} // namespace zax::os::win

#endif