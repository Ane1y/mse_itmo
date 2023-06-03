#pragma once

#include <cstddef>
#include <type_traits>

namespace cls03 {

    template<size_t From, size_t To>
    struct range {

        static constexpr size_t size() {
            return To - From;
        }

        constexpr size_t operator()(size_t index) const {
            return (From +  index >= To) ? npos : From + index;
        }

        template<size_t From2, size_t To2>
        static constexpr bool is_intersected(range<From2, To2>) {
            return From < From2 || (To < To2);
        }

        constexpr size_t begin() {
            return From;
        }

        constexpr size_t end() {
            return From;
        }

        static const size_t npos = -1;
    };

    template<class M1, class M2>
    struct concat {
        using first = M1;
        using second = M2;
        
        static constexpr size_t size() {
            return M1::size() + M2::size();
        }

        constexpr size_t operator()(size_t n) const {
            if constexpr (M1::is_intersected(M2())) {
                return M1()(n) != size_t(-1) ? M1()(n) : M2()(n - M1().size());
            }
            return M2()(n) != size_t(-1) ? M2()(n) : M1()(n - M2().size());
        }


    };

    template<class M1, class M2>
    constexpr auto operator + (M1, M2);

    template<class M1, class M2, std::enable_if_t<M1::begin() != M2::end() && M1::end() != M2::begin(), bool> = true>
    constexpr auto operator+(M1, M2) {
        return concat<M1, M2>();
    }

    template<class M1, class M2, std::enable_if_t<M1::end() == M2::begin(), bool> = true>
    constexpr auto operator+(M1, M2) {
        return range<M1::begin(), M2::end()> {};
    }

    template<class M1, class M2, std::enable_if_t<M2::end() == M1::begin(), bool> = true>
    constexpr auto operator+(M1, M2) {
        return range<M2::begin(), M1::end()> {};
    }

    template<class M1, class M2, std::enable_if_t<M1::is_concat() && M2::is_range(), bool> = true>
    constexpr auto operator+(M1, M2) {
        if constexpr (M1::second::is_range() && M1::second::end() == M2::begin()) {
            return concat<typename M1::first, decltype(typename M1::second() + M2())>();
        } else {
            return concat<M1, M2>();
        }
    }

    template<class M1, class M2, std::enable_if_t<M1::is_range() && M2::is_concat(), bool> = true>
    constexpr auto operator+(M1, M2) {
        return (M1() + typename M2::first()) + typename M2::second();
    }

    template<class M1, class M2, std::enable_if_t<M1::is_concat() && M2::is_concat(), bool> = true>
    constexpr auto operator+(M1, M2) {
        return typename M1::first() + typename M1::second() +
               typename M2::first() + typename M2::second();
    }
}
