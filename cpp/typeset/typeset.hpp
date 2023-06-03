#pragma once
#include <cstddef>
#include <type_traits>
#include <variant>

namespace cls03 {
    template<class...> struct typeset {};

    namespace details {
        template<class Typeset>
        struct size;

        template <class... Args>
        struct size<typeset<Args...>> {
            static const size_t value = sizeof...(Args);
        };


        template<size_t N, class Typeset>
        struct get;

        template <size_t N, class Head, class... Tail>
        struct get<N, typeset<Head, Tail...>> {
            using type = typename get<N - 1, typeset<Tail...>>::type;
        };

        template <class Head, class ...Tail>
        struct get<0, typeset<Head, Tail...>> {
            using type = Head;
        };


        template<class T, class Typeset>
        struct belongs_to;

        template<class T, class Head, class ...Tail>
        struct belongs_to<T, typeset<Head, Tail...>>{
            static const bool value = std::is_same_v<T, Head> || belongs_to<T, typeset<Tail...>>::value;
        };

        template<class T>
        struct belongs_to<T, typeset<>>{
            static const bool value = false;
        };


        template<class Ts1, class Ts2>
        struct join;

        template<class ...Args1, class... Args2>
        struct join<typeset<Args1...>, typeset<Args2...>> {
            using type = typeset<Args1..., Args2...>;
        };


        template<class Ts1, class Ts2>
        struct cross;

        template<class Head, class... Tail, class Typeset>
        struct cross<typeset<Head, Tail...>, Typeset> {
            using type =  typename join<std::conditional_t<belongs_to<Head, Typeset>::value,
                                                            typeset<Head>,
                                                            typeset<>>,
                                        typename cross<typeset<Tail...>, Typeset>::type>::type;
        };

        template<class Typeset>
        struct cross<typeset<>, Typeset> {
            using type = typeset<>;
        };


        template<class Ts1, class Ts2>
        struct substract;

        template<class Head, class... Tail, class Typeset>
        struct substract<typeset<Head, Tail...>, Typeset> {
            using type =  typename join<std::conditional_t<!belongs_to<Head, Typeset>::value,
                    typeset<Head>,
                    typeset<>>,
                    typename substract<typeset<Tail...>, Typeset>::type>::type;
        };

        template<class Typeset>
        struct substract<typeset<>, Typeset> {
            using type = typeset<>;
        };


        template<class Typeset>
        struct variant_of;

        template<class...Args>
        struct variant_of<typeset<Args...>> {
            using type = std::variant<Args...>;
        };
    }

    template<class Typeset>
    constexpr bool is_empty = details::size<Typeset>::value == 0;

    template<class Typeset>
    constexpr size_t size = details::size<Typeset>::value;

    template<size_t N, class Typeset>
    using get = typename details::get<N, Typeset>::type;

    template<class T, class Typeset>
    constexpr bool belongs_to = details::belongs_to<T, Typeset>::value;
    template<class Ts1, class Ts2>
    using join = typename details::join<Ts1, Ts2>::type;

    template<class Ts1, class Ts2>
    using cross = typename details::cross<Ts1, Ts2>::type;

    template<class Ts1, class Ts2>
    using subtract = typename details::substract<Ts1, Ts2>::type;

    template<class Typeset>
    using variant_of = typename details::variant_of<Typeset>::type;
}