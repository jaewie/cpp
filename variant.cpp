#include "traits.hpp"
#include <cassert>

// Mostly from https://gist.github.com/tibordp/6909880

template<typename T, typename... Args>
struct VariantUtils {
    inline static void copy(void *source, const void *target, size_t type) {
        if (typeid(T).hash_code() == type) {
            new (source) T(*reinterpret_cast<const T*>(target));
        } else {
            VariantUtils<Args...>::copy(source, target, type);
        }
    }

    inline static void move(void *source, void *target, size_t type) {
        if (typeid(T).hash_code() == type) {
            new (source) T(std::move(*reinterpret_cast<T*>(target)));
        } else {
            VariantUtils<Args...>::move(source, target, type);
        }
    }

    inline static void destroy(void *source, size_t type) {
        if (typeid(T).hash_code() == type) {
            reinterpret_cast<T&>(source).~T();
        } else {
            VariantUtils<Args...>::destroy(source, type);
        }
    }
};

template<typename T>
struct VariantUtils<T> {
    inline static void copy(void *source, const void *target, size_t type) {
        if (typeid(T).hash_code() == type) {
            new (source) T(*reinterpret_cast<const T*>(target));
        }
    }

    inline static void move(void *source, void *target, size_t type) {
        if (typeid(T).hash_code() == type) {
            new (source) T(std::move(*reinterpret_cast<T*>(target)));
        }
    }

    inline static void destroy(void *source, size_t type) {
        if (typeid(T).hash_code() == type) {
            reinterpret_cast<T&>(source).~T();
        }
    }
};


template<typename... Args>
struct Variant {
    uint8_t bytes[max_size_v<Args...>];
    size_t type;

    Variant() = default;
    Variant(const Variant<Args...> &v) {
       VariantUtils<Args...>::copy(&bytes, &v.bytes, v.type);
       type = v.type;
    }

    Variant(Variant<Args...> &&v) {
       VariantUtils<Args...>::move(&bytes, &v.bytes, v.type);
       type = v.type;
    }

    template<typename T>
    requires is_one_of_v<std::remove_reference_t<T>, Args...>
    Variant(T&& arg) {
       new(bytes) T(std::forward<T>(arg));
       type = typeid(T).hash_code();
    }

    ~Variant() {
       VariantUtils<Args...>::destroy(&bytes, type);
    }

    template<typename T, typename... TArgs>
    void emplace(TArgs... args) {
        new(bytes) T(std::forward<TArgs>(args)...);
    }

    template<typename T>
    T& get() {
        return reinterpret_cast<T&>(bytes);
    }
};

struct Widget {
    int val = 42;
};

int main() {
    Variant<int, uint8_t, string> x = 32;
    Variant<int, uint8_t, string> y = x;
    Variant<int, uint8_t, string> z = std::move(x);

    y.get<int>() = 42;

    cout << x.get<int>() << endl;
    cout << y.get<int>() << endl;
    cout << z.get<int>() << endl;

}