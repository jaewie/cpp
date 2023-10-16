#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdio.h>
#include <string.h>

using namespace std;

template<typename F, typename S>
struct Pair {
   uint8_t bytes[sizeof(F) + sizeof(S)];

   Pair() = default;

   template<typename X, typename Y>
   Pair(X&& first, Y&& second) noexcept {
      static_assert(is_same_v<F, remove_reference_t<X>>);
      static_assert(is_same_v<S, remove_reference_t<Y>>);
      reinterpret_cast<F&>(bytes) = std::forward<X>(first);
      reinterpret_cast<S&>(bytes[sizeof(first)]) = std::forward<Y>(second);
   }

   F& first() noexcept {
      return reinterpret_cast<F&>(bytes);
   }

   const F& first() const noexcept {
      return reinterpret_cast<const F&>(bytes);
   }

   S& second() noexcept {
      return reinterpret_cast<F&>(bytes + sizeof(F));
   }

   const S& second() const noexcept {
      return reinterpret_cast<const S&>(bytes[sizeof(F)]);
   }
};

int main() {
    Pair<int, double> p = {1, 3.2};
    p.first() = 42;

    cout << p.first();

}