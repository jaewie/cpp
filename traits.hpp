#include <iostream>

using namespace std;

template<typename X, typename... Args>
struct max_size;

template<typename X, typename Y, typename... Args>
struct max_size<X, Y, Args...> {
   static constexpr size_t value = sizeof(X) < sizeof(Y) ? max_size<Y, Args...>::value : max_size<X, Args...>::value;
};

template<typename X>
struct max_size<X> {
   static constexpr size_t value = sizeof(X);
};

template<typename X, typename... Args>
static constexpr size_t max_size_v = max_size<X, Args...>::value;


template<size_t Idx, typename T, typename... Args>
struct index_of {
   static constexpr size_t value = -1;
};

template<size_t Idx, typename T, typename... Args>
struct index_of<Idx, T, T, Args...> {
   static constexpr size_t value = Idx;
};

template<size_t Idx, typename T, typename U, typename... Args>
struct index_of<Idx, T, U, Args...> {
   static constexpr size_t value = index_of<Idx + 1, T, Args...>::value;
};

template<typename T, typename... Args>
static constexpr size_t index_of_v = index_of<0, T, Args...>::value;

template<typename T, typename... Args>
static constexpr bool is_one_of_v = index_of_v<T, Args...> != -1;