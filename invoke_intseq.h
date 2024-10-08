/*
 * =====================================================
 * Czwarty projekt zaliczeniowy z
 * przedmiotu Kurs Programowania w C++ (1000-213bCPP).
 *
 * Plik nagłówkowy modułu 'invoke_intseq'.
 *
 * Plik zawiera implementacje szablonów wykorzystywanych
 * przy realizacji operacji określonych w specyfikacji.
 *
 * Autorzy:
 * Dawid Pawlik    <dp450244@students.mimuw.edu.pl>
 * Anna Piórkowska <ap448450@students.mimuw.edu.pl>
 * =====================================================
 */


/* Początek deklaracji elementów modułu invoke_intseq.h */

#ifndef INVOKE_INTSEQ_H
#define INVOKE_INTSEQ_H


/* Pliki nagłówkowe wykorzystywane do implementacji modułu */

#include <functional>


/* Deklaracja użycia wybranych elementów z przestrzeni std */

using std::integer_sequence;
using std::invoke;
using std::array;
using std::forward;
using std::is_same_v;


/* Ukrywane szablony funkcji i struktur przestrzeni cxx */

namespace cxx {

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

    template <typename T, T... t>
    struct intseq_size<integer_sequence<T, t...>> {
        static constexpr size_t size() {
            return sizeof...(t);
        }
    };

    template <typename T>
    constexpr size_t check([[maybe_unused]] T arg) {
        return 1;
    }

    template <intseq T>
    constexpr size_t check(T arg) {
        return intseq_size<decltype(arg)>::size();
    }

    template <typename... T>
    constexpr size_t arraySize(T&&... args) {
        return (check(args) * ...);
    }

    template <size_t pos, typename T>
    struct find_element {
    };

    template <size_t pos, typename T, T t0>
    struct find_element<pos, integer_sequence<T, t0>> {
        static constexpr T run() {
            return t0;
        }
    };

    template <size_t pos, typename T, T t0, T... t>
    struct find_element<pos, integer_sequence<T, t0, t...>> {
        static constexpr T run() {
            if constexpr (!pos) {
                return find_element<0, integer_sequence<T, t0>>::run();
            } else {
                return find_element<pos - 1, integer_sequence<T, t...>>::run();
            }
        }
    };

    template <typename... T>
    struct produce {};

    template <>
    struct produce<> {
        template <typename F, typename... Checked>
        static constexpr decltype(auto) correct_invoke(F&& f, Checked&&... args) {
            invoke(forward<F>(f), forward<Checked>(args)...);
        }

        template <typename F, typename... Checked>
        static constexpr decltype(auto) one_invoke(F&& f, Checked&&... args) {
            return invoke(forward<F>(f), forward<Checked>(args)...);
        }
    };

    template <typename T, typename... Remaining>
    struct produce<T, Remaining...> {
        template <typename F, typename... Checked>
        static constexpr decltype(auto) correct_invoke(F&& f, T arg, Checked&&... args) {
            produce<Remaining...>::correct_invoke(forward<F>(f), forward<Checked>(args)..., forward<T>(arg));
        }

        template <typename F, typename... Checked>
        static constexpr decltype(auto) one_invoke(F&& f, T arg, Checked&&... args) {
            return produce<Remaining...>::one_invoke(forward<F>(f), forward<Checked>(args)..., forward<T>(arg));
        }
    };

    template <typename T, T t0, T... t, typename... Remaining>
    struct produce<integer_sequence<T, t0, t...>, Remaining...> {
        template <typename F, typename... Checked>
        static constexpr decltype(auto) correct_invoke(F&& f, integer_sequence<T, t0, t...>&&, Checked&&... args) {
            produce<Remaining...>::correct_invoke(forward<F>(f), forward<Checked>(args)..., forward<T>(t0));
            if constexpr (sizeof...(t)) {
                produce<integer_sequence<T, t...>, Remaining...>::correct_invoke(forward<F>(f), integer_sequence<T, t...>(), forward<Checked>(args)...);
            }
        }

        template <typename F, typename... Checked>
        static constexpr decltype(auto) one_invoke(F&& f, integer_sequence<T, t0, t...>&&, Checked&&... args) {
            return produce<Remaining...>::one_invoke(forward<F>(f), forward<Checked>(args)..., forward<T>(t0));
        }
    };


} // Koniec przestrzeni nazw cxx.


/* Implementowany szablon funkcji */

/*
 * Funkcja wywołuje funkcję 'f' z argumentami 'args' i w zależności od typu
 * przekazywanego przez funkcję 'f' przekazuje void, gdy funkcja nie przekazuje
 * żadnego typu, przekazuje wynik funkcji 'f', gdy funkcja przekazuje typ lub
 * kontener, po którym można iterować zawierający wyniki rekurencyjnych wywołań
 * funkcji 'f' dla wszystkich kombinacji 'integer_sequence' zawartych w 'args'.
 */
template <typename F, typename... Args>
constexpr decltype(auto) invoke_intseq(F&& f, Args&&... args) {
    using invoke_type = decltype(cxx::produce<Args...>::one_invoke(forward<F>(f), forward<Args>(args)...));
    if constexpr (is_same_v<invoke_type, void>) {
        // to po prostu wywołujemy i łatwo
        cxx::produce<Args...>::correct_invoke(forward<F>(f), forward<Args>(args)...);
    } else {
        // -_- Array zrobić
        // constexpr size_t n = cxx::arraySize(args...);
        // array<invoke_type, n> results;

        return 0;
    }
}


#endif // Koniec header-guard.