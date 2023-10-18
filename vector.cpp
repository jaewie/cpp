#include "traits.hpp"

using namespace std;

template<typename T>
struct Vector {
    static constexpr size_t initial_capacity_ = 16;
    uint8_t initial_arr_[initial_capacity_ * sizeof(T)];

    uint8_t *arr_;
    size_t capacity_;
    size_t size_;

    Vector(): arr_(initial_arr_), capacity_(initial_capacity_), size_(0) {
    }

    Vector(size_t n, T t): capacity_(next_size(n)), arr_(n <= initial_capacity_ ? initial_arr_ : new uint8_t[next_size(n) * sizeof(T)]), size_(0) {
        for (int i = 0; i < n; i++) {
            new(arr_ + size_++ * sizeof(T)) T(t);
        }
    }

    template<typename A>
    requires is_same_v<T, remove_reference_t<A>>
    void push_back(A&& t) {
        expand_if_full();
        new(arr_ + size_++ * sizeof(T)) T(std::forward<T>(t));
    }

    template<typename... Args>
    void emplace(Args&&... t) {
        expand_if_full();
        new(arr_ + size_++ * sizeof(T)) T(std::forward<Args>(t)...);
    }

    void expand_if_full() {
        if (size_ == capacity_) {
            reserve(capacity_ * 2, initial_arr_ != arr_);
        }
    }

    T& operator[](size_t i) {
        return reinterpret_cast<T&>(arr_[i * sizeof(T)]);
    }

    void reserve(size_t n, bool free_arr = true) {
        n = next_size(n);
        auto new_arr = new uint8_t[n * sizeof(T)];
        for (int i = 0; i < size_; i++) {
            new(new_arr + i * sizeof(T)) T(std::move_if_noexcept(reinterpret_cast<T&>(arr_[i * sizeof(T)])));
        }

        if (free_arr) {
            free(arr_);
        }
        arr_ = new_arr;
        capacity_ = n;
    }

    static constexpr size_t next_size(size_t n) {
        return 1 << ((size_t) ceil(log2(n)));
    }

    size_t size() {
        return size_;
    }
};

struct Widget {
    int x;
    int y;

    Widget(int a, int b): x(a), y(b) {
        cout << "cstr" << endl;
    }

    Widget(const Widget& v) noexcept {
        cout << "copy cstr" << endl;
        x = v.x;
        y = v.y;
    }

    Widget(Widget &&v) noexcept {
        cout << "move cstr" << endl;
        x = v.x;
        y = v.y;
    }
};

int main() {
    vector<Widget> v(10, Widget{32, 2});

    cout << v[3].x;

}