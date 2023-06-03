#pragma once

#include <iostream>
#include <type_traits>
#include <iomanip>

namespace cls03 {

    namespace details {

        template<class, class = void>
        struct is_complete : std::false_type {};

        template<class T>
        struct is_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};

        template< class T >
        inline constexpr bool is_complete_v = is_complete<T>::value;

        template<class, class = void>
        struct is_printable : std::false_type {};

        template<class T>
        struct is_printable<T, std::void_t<decltype(void(std::declval<std::ostream &>() << std::declval<const T &>()))>> : std::true_type {};

        template< class T >
        inline constexpr bool is_printable_v = is_printable<T>::value;
    }

    template <class T, std::enable_if_t<!details::is_complete_v<T>
            , bool> = true>
    void pretty_printer(std::ostream& ost, const T&) {
        ost << "incomplete";
    }

    template<class T, std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, bool> = true>
    void pretty_printer(std::ostream& ost, const T& t) {
        ost << "signed int_" << sizeof(t) * 8 << " " << static_cast<int64_t>(t);
    }


    template<class T, std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, bool> = true>
    void pretty_printer(std::ostream& ost, const T& t) {
        ost << "unsigned int_" << sizeof(t) * 8 << " " << static_cast<uint64_t>(t);
    }

    template<class T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
    void pretty_printer(std::ostream& ost, const T& t) {
        std::string type = (sizeof(t) <= sizeof(float)) ? "float" : "double";
        ost << type << " " << std::fixed << std::setprecision(6) << t;
    }

    template<class T, std::enable_if_t<!std::is_integral_v<T>
                                       && !std::is_floating_point_v<T>
                                       && details::is_printable_v<T>
                                       && details::is_complete_v<T>
                                        , bool> = true>
    void pretty_printer(std::ostream& ost, const T& t) {
        ost << "some printable " << t;
    }

    template<class T, std::enable_if_t<!std::is_integral_v<T>
                                       && !std::is_floating_point_v<T>
                                       && details::is_complete_v<T>
                                       && !details::is_printable_v<T>
                                       , bool> = true>
    std::enable_if_t<std::is_standard_layout_v<T>, void> pretty_printer(std::ostream& ost, const T& t) {
        ost << sizeof(t) << " bytes";
        for(size_t i = 0; i < sizeof(T); ++i) {
            ost << " " << static_cast<int64_t>(*(reinterpret_cast<const char*>(&t) + i));
        }
    }

    template <class T, std::enable_if_t<!std::is_integral_v<T>
                                        && !std::is_floating_point_v<T>
                                        && !details::is_printable_v<T>
                                        && details::is_complete_v<T>
                                        , bool> = true>
    std::enable_if_t<!std::is_standard_layout_v<T>, void> pretty_printer(std::ostream& ost, const T&) {
        ost << "Some " << sizeof(T) << " bytes";
    }
}