//
#ifndef ZAX_READER_HPP
#define ZAX_READER_HPP
#include "zax.hpp"
#include <vector>
namespace zax {
class Reader {
public:
  virtual ssize_t Read(uint8_t *buffer, size_t len) = 0;
};
class Writer {
public:
  virtual ssize_t Write(const uint8_t *data, size_t len) = 0;
};
class MultiWriter : public Writer {
public:
  ssize_t Write(const uint8_t *data, size_t len) {
    size_t n = 0;
    for (auto &w : mw) {
      if (n = w->Write(data, len); n < 0) {
        break;
      }
    }
    return n;
  }
  template <typename... W> static MultiWriter MakeWriter(W... w) {
    MultiWriter mw;
    const Writer *wv[] = {w...};
    for (auto i : mv) {
      mw.mw.emplace_back(i);
    }
    return mw;
  }

private:
  std::vector<Writer *> mw;
};
} // namespace zax

#endif