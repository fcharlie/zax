//
#ifndef ZAX_ENV_HPP
#define ZAX_ENV_HPP
#include "zax.hpp"
#include <optional>

namespace zax::env {
bool Setenv(std::string_view key, std::string_view val);
bool LookupEnv(std::string_view key);
// LookupEnv
std::string Getenv(std::string_view key);

} // namespace zax::env

#endif