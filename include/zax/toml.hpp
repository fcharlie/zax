//
#ifndef ZAX_TOML_HPP
#define ZAX_TOML_HPP
#include <string_view>
#include <string>
#include <charconv>
#include "zax.hpp"
//#include <unordered_map>

// https://github.com/toml-lang/toml/wiki
namespace zax::toml {
// struct string_compare {
//   using is_transparent = void; // Pred to use
//   bool operator()(const std::string &key, std::string_view txt) const { return key == txt; }
//   bool operator()(std::string_view key, std::string_view txt) const { return key == txt; }
// };

// struct string_hash {
//   using is_transparent = void; // <-- It does not work without this trait
//   size_t operator()(std::string_view txt) const { return std::hash<std::string_view>{}(txt); }
//   size_t operator()(const std::string &txt) const { return std::hash<std::string>{}(txt); }
// };
// inline void mapfind() {

//   std::unordered_map<std::string, std::string, string_hash, string_compare> kv;
//   std::string_view k = "k1";
//   auto it = kv.find(k);
//   if (it != kv.end()) {
//   }
// }

// toml table
class Table {};
} // namespace zax::toml

#endif