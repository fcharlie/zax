//
#ifndef ZAX_HPP
#define ZAX_HPP
#include <cstdint>
#include <string_view>

namespace zax {
// final_act
// https://github.com/microsoft/gsl/blob/ebe7ebfd855a95eb93783164ffb342dbd85cbc27\
// /include/gsl/gsl_util#L85-L89

template <class F> class final_act {
public:
  explicit final_act(F f) noexcept : f_(std::move(f)), invoke_(true) {}

  final_act(final_act &&other) noexcept
      : f_(std::move(other.f_)), invoke_(std::exchange(other.invoke_, false)) {}

  final_act(const final_act &) = delete;
  final_act &operator=(const final_act &) = delete;
  ~final_act() noexcept {
    if (invoke_) {
      f_();
    }
  }

private:
  F f_;
  bool invoke_{true};
};

// finally() - convenience function to generate a final_act
template <class F> inline final_act<F> finally(const F &f) noexcept { return final_act<F>(f); }

template <class F> inline final_act<F> finally(F &&f) noexcept {
  return final_act<F>(std::forward<F>(f));
}

} // namespace zax

#endif