///
#ifndef ZAX_ARGS_HPP
#define ZAX_ARGS_HPP
#include <string_view>
#include <vector>
#include <functional>
#include "zax.hpp"

namespace zax::args {
// zax::args::ParseArgs
// Thanks https://github.com/fcharlie/bela/blob/master/include/bela/parseargv.hpp
enum HasArgs {
  required_argument, /// -i 11 or -i=xx
  no_argument,
  optional_argument /// -s --long --long=xx
};
struct option {
  option() = default;
  option(std::string_view n, HasArgs ha, int va)
      : name(n), has_args(ha), val(va) {} /// emplace_back
  std::string_view name;
  HasArgs has_args;
  int val;
};
using invoke_t = std::function<bool(int, const char *, const char *)>;
class ParseArgs {
public:
  ParseArgs(int argc, char *const *argv, bool subcmd = true)
      : argv_(argv), argc_(argc), subcmd_(subcmd) {}
  ParseArgs(const ParseArgs &) = delete;
  ParseArgs &operator=(const ParseArgs &) = delete;
  ParseArgs &Add(std::string_view name, HasArgs a, int val) {
    options_.emplace_back(name, a, val);
    return *this;
  }
  bool Execute(invoke_t &v, zax::error_code &ec);
  const std::vector<std::string_view> &SubArgs() const { return subargs; }

private:
  std::vector<option> options_;
  std::vector<std::string_view> subargs;
  const char *const *argv_;
  const int argc_;
  int index{0};
  bool subcmd_;
  bool parse_internal(std::string_view a, const invoke_t &v, zax::error_code &ec);
  bool parse_internal_long(std::string_view a, const invoke_t &v, zax::error_code &ec);
  bool parse_internal_short(std::string_view a, const invoke_t &v, zax::error_code &ec);
};
} // namespace zax::args

#endif