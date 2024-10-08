#include <bits/stdc++.h>
using namespace std;

namespace {
    template <typename T>
    struct is_intseq {
        constexpr static bool value = false;
    };

    template <typename T, T... args>
    struct is_intseq<integer_sequence<T, args...>> {
        constexpr static bool value = true;
    };

    template <typename T>
    concept intseq = is_intseq<T>::value;


    template <typename T>
    struct intseq_size {
    };


    template <typename T, T t0>
    struct intseq_size<integer_sequence<T, t0>> {
        static constexpr size_t size() {
            return 1;
        }
    };

    template <typename T, T t0, T... t>
    struct intseq_size<integer_sequence<T, t0, t...>> {
        static constexpr size_t size() {
            return 1 + intseq_size<integer_sequence<T, t...>>::size();
        }
    };

    template <typename T>
    constexpr size_t check(T arg) {
        return 1;
    }

    template <intseq T>
    constexpr size_t check(T arg) {
        return intseq_size<decltype(arg)>::size();
    }

    template <typename... T>
    constexpr void print_type(T&&... args) {
        cout << (check(args) * ...) << endl;
    }

} /* anonymous namespace */

int main() {
    print_type(2, integer_sequence<int, 3, 4, 5>(), 'c', integer_sequence<char, 'f', 'f'>(), "Ania");
}