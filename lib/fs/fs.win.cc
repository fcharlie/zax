//
#include <zax/fs.hpp>

namespace zax::fs {
File::~File() {
  //
  Close();
}
int File::Close() {
  if (fd != closedfd) {
    CloseHandle(fd);
    fd = closedfd;
  }
  return 0;
}

ssize_t File::Write(const uint8_t *data, size_t len) {
  //
  return -1;
}
ssize_t File::Read(uint8_t *buffer, size_t bufsz) {
  //
  return -1;
}

} // namespace zax::fs