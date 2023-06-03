#pragma once

#include <iostream>
#include <type_traits>
#include <cstring>
#include <iomanip>
#include <memory>

namespace cls_08 {
    namespace details {

        template<class T> concept Pointer = requires(T ptr) {
            *ptr;
        };
        template<class T> concept CString = std::is_same_v<std::decay_t<T>, char *> || std::is_same_v<std::decay_t<T>, const char *> ;

        template<class T>
        constexpr bool helper();
        template<class T> concept Printable = helper<T>();
    }
    template<class T> concept PrettyPrintable = !std::same_as<std::decay_t<T>, void *> && details::Printable<T>;

    template<class T> concept Outputtable = (!details::Pointer<T> || details::CString<T>)
                                            && !std::same_as<std::decay_t<T>, void *>
                                            && requires(std::ostream &out, const T& t) {
        out << t;
    };

    template<class T, class U = std::decay_t<T>> concept Binary = std::is_trivial_v<U>
                                        && std::is_standard_layout_v<U>
                                        && !std::same_as<std::decay_t<T>, void *>
                                        && !Outputtable<T>
                                        && !details::Pointer<T>;

    template<class T> concept PrettyPrintablePointer =  details::Pointer<T>
                                                        && !details::CString<T>
                                                        && !std::is_same_v<std::decay_t<T>, void *>
                                                        && requires(const T& t) {
        {*t} -> PrettyPrintable;
    };

    template<class T> concept PrettyPrintableCollection = requires(const T& t) {
        { *std::begin(t) } -> PrettyPrintable;
    };


    template<class T>
    constexpr bool details::helper() {
        return PrettyPrintablePointer<T> || Outputtable<T> || Binary<T> || PrettyPrintableCollection<T>;
    }
    ////////////////////////////////////////////

    template<Outputtable T>
    void pretty_print(std::ostream& ost, const T& t);

    template<Binary T>
    void pretty_print(std::ostream& ost, const T& t);

    template<PrettyPrintablePointer T>
    void pretty_print(std::ostream& ost, const T& t);

    template<PrettyPrintableCollection T> requires (!Outputtable<T>)
    void pretty_print(std::ostream& ost, const T& t);


    template<Outputtable T>
    void pretty_print(std::ostream& ost, const T& t) {
        ost << t;
    }


    template<Binary T>
    void pretty_print(std::ostream& ost, const T& t) {
        ost << "0" << static_cast<int64_t>(*(reinterpret_cast<const char*>(&t)));
        for(size_t i = 1; i < sizeof(T); ++i) {
            ost << "," << "0" << static_cast<int64_t>(*(reinterpret_cast<const char*>(&t) + i));
        }
    }

    template<PrettyPrintablePointer T>
    void pretty_print(std::ostream& ost, const T& t) {
        if (t == nullptr) {
            ost << "nullptr";
        } else {
            ost << "*";
            pretty_print(ost, *t);
        }

    }
    template<PrettyPrintableCollection T> requires (!Outputtable<T>)
    void pretty_print(std::ostream& ost, const T& t) {
        ost << "{ ";
        for (const auto& it: t) {
            pretty_print(ost, it);
            ost << " ";
        }
        ost << "}";
    }

    //////////////////////////////////////////////////////

    template<class T, bool Pretty> concept SomehowPrintable = ((Pretty && PrettyPrintable<T>) || (!Pretty && !PrettyPrintable<T>))
                                                                && requires(std::ostream& ost, const T& t) {
        ost << t;
    };


    template <SomehowPrintable<true> T>
    void somehow_print(std::ostream& ost, const T& t) {
        pretty_print(ost, t);
    }

    template <SomehowPrintable<false> T>
    void somehow_print(std::ostream& ost, const T& t) {
        ost << t;
    }
}  // namespace cls_08
