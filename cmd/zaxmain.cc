//
#include <vector>
#include <cstdint>
#include <zax/zax.hpp>
#include <zax/unicode.hpp>

int ZaxMain(int argc, char **argv); // ZaxMain command entrypoint

#ifdef _WIN32

inline bool ZaxNarrowArgv(int argc, wchar_t **argv,
                          std::vector<char *> &narrowargv) {
  narrowargv.reserve(argc);
  for (auto i = 0; i < argc; i++) {
    auto p = zax::text::ToNarrow(argv[i]);
    if (p == nullptr) {
      return false;
    }
    narrowargv.emplace_back(p);
  }
  return true;
}

int wmain(int argc, wchar_t **argv) {
  std::vector<char *> narrowargv;
  if (!ZaxNarrowArgv(argc, argv, narrowargv)) {
    return 127;
  }
  auto releaser = zax::finally([&] {
    for (auto p : narrowargv) {
      free(p);
    }
  });
  return ZaxMain(static_cast<int>(narrowargv.size()), narrowargv.data());
}
#else
int main(int argc, char **argv) {
  //
  return ZaxMain(argc, argv);
}
#endif