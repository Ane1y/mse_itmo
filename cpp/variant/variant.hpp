#pragma once
#include <variant>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <new>
namespace util {

    template<class ...Ts>
    class variant;

    size_t variant_npos = -1;

    class bad_get : public std::runtime_error {
    public:
        bad_get(const std::string& what) : std::runtime_error(what) {}
    };


    template<class T, class ...Args>
    T& get(variant<Args...>& v);

    template<class T, class ...Args>
    const T& get(const variant<Args...>& v);

    template<class T, class ...Args>
    T&& get(variant<Args...>&& v);

    template<size_t I, class... Args>
    decltype(auto) get(variant<Args...>& v);

    template<size_t I, class... Args>
    decltype(auto) get(const variant<Args...>& v);

    template<size_t I, class... Args>
    decltype(auto) get(variant<Args...>&& v);

    template<size_t I, class... Args>
    decltype(auto) get(variant<Args...>* v);

    template<size_t I, class... Args>
    decltype(auto) get(const variant<Args...>* v);

    namespace detail {

        template<size_t Idx, size_t CurIdx, typename T, typename... Ts>
        struct get_type_impl {
            using type = typename get_type_impl<Idx, CurIdx + 1, Ts...>::type;
        };

        template<typename T>
        struct get_type_impl<0, 0, T> {
            using type = T;
        };

        template<size_t Idx, typename T, typename... Ts>
        struct get_type_impl<Idx, Idx, T, Ts...> {
            using type = T;
        };


        template<size_t Idx, typename... Ts>
        struct get_type {
            using type = typename get_type_impl<Idx, 0, Ts...>::type;
        };

        template<size_t Idx, typename... Ts>
        using get_type_t = typename get_type<Idx, Ts...>::type;

        template<typename Search, size_t Idx, typename... Types>
        struct get_idx;

        template<typename Type, size_t Idx, typename T, typename... Ts>
        struct get_idx <Type, Idx, T, Ts...>{
            static constexpr size_t value = get_idx<Type, Idx + 1, Ts...>::value;
        };

        template<typename Type, size_t Idx, typename... Types>
        struct get_idx<Type, Idx, Type, Types...> {
            static constexpr size_t value = Idx;
        };

        template<typename Type, typename T, typename... Ts>
        inline constexpr bool get_idx_v = get_idx<Type, 0, T, Ts...>::value;

        template<class ...Ts>
        struct helper;

        template<class T, class ...Ts>
        struct helper<T, Ts...> {

            template<class Type, size_t N>
            static bool create(std::byte* buffer, Type&& obj, size_t& type_idx) {
                if constexpr(std::is_same_v<T, std::decay_t<Type>>) {
                    new(buffer) std::decay_t<Type>(std::forward<Type>(obj));
                    type_idx = N;
                    return true;
                }
                return detail::helper<Ts...>::template create<Type, N + 1>(buffer, std::forward<Type>(obj), type_idx);
            }


            template<class Type, size_t N>
            static bool convert(std::byte* buffer, Type&& obj, size_t& type_idx) {
                    if constexpr (std::is_convertible_v<std::decay_t<Type>, T>) {
                        new(buffer) std::decay_t<T>(std::forward<Type>(obj));
                        type_idx = N;
                        return true;
                    }
                    return helper<Ts...>::template convert<Type, N + 1>(buffer, std::forward<Type>(obj), type_idx);

            }

            template<class Type, size_t TypeIdx>
            static Type* get(std::byte* buffer, size_t cur_idx) {
                if (cur_idx == TypeIdx) {
                    return reinterpret_cast<Type *>(buffer);
                }
                return helper<Ts...>::template get<Type, TypeIdx>(buffer, cur_idx++);
            }


            template<size_t Idx>
            static void destroy(std::byte* buffer, size_t& type_idx) {
                if (Idx == type_idx) {
                    reinterpret_cast<T*>(buffer)->~T();
                    type_idx = util::variant_npos;
                } else {
                    helper<Ts...>::template destroy<Idx + 1>(buffer, type_idx);
                }

            }


            template<size_t Idx>
            static void move(std::byte* buffer, std::byte* other_buffer, size_t& other_type_idx) {
                if (Idx == other_type_idx) {
                    new(buffer) T(std::move(*reinterpret_cast<T *>(other_buffer)));
                    other_type_idx = util::variant_npos;
                }
                helper<Ts...>::template move<Idx + 1>(buffer, other_buffer, other_type_idx);
            }

            template<size_t Idx>
            static void copy(std::byte* buffer, const std::byte* other_buffer, size_t type_idx) {
                if (Idx == type_idx) {
                    new(buffer) T(*std::launder(reinterpret_cast<const T*>(other_buffer)));
                }
                helper<Ts...>::template copy<Idx + 1>(buffer, other_buffer, type_idx);
            }


            template <size_t Idx, class Visitor, class ...Args>
            static decltype(auto) apply_visitor(Visitor&& visitor, variant<Args...>& v) {
                if (v.index() == Idx) {
                    return visitor(util::get<T>(v));
                }
                return helper<Ts...>::template apply_visitor<Idx + 1, Visitor>(std::forward<Visitor>(visitor), v);
            }

            template <size_t Idx, class Visitor, class ...Args>
            static decltype(auto) apply_visitor(Visitor&& visitor, const variant<Args...>& v) {
                if (Idx == v.index()) {
                    return visitor(util::get<T>(v));
                }

                return helper<Ts...>::template apply_visitor<Idx + 1, Visitor>(std::forward<Visitor>(visitor), v);
            }

            template <size_t Idx, class Visitor, class ...Args>
            static decltype(auto) apply_visitor(Visitor&& visitor, variant<Args...>&& v) {
                if (Idx == v.index()) {
                    return visitor(std::move(util::get<T>(v)));
                }
                return helper<Ts...>::template apply_visitor<Idx + 1, Visitor>(std::forward<Visitor>(visitor), std::move(v));
            }
        };

        template<>
        struct helper<> {
            template<class Type, size_t N>
            static bool create(std::byte*, Type&&, size_t&) {return false;}
            template<class Type, size_t N>
            static bool convert(std::byte*, Type&&, size_t&) {return false;}
            template <class Type, size_t N>
            static Type* get(std::byte*, size_t) {
                return nullptr;
            }
            template<size_t Idx>
            static void move(std::byte*, std::byte*, size_t&) {}

            template<size_t Idx>
            static void copy(std::byte*, const std::byte*, size_t) {}
            template <size_t Idx>
            static void destroy(std::byte*, size_t&) {}

            template <size_t Idx, class Visitor, class ...Args>
            static decltype(auto) apply_visitor(Visitor&& visitor, variant<Args...>& ) {
                return visitor();
            }

            template <size_t Idx, class Visitor, class ...Args>
            static decltype(auto) apply_visitor(Visitor&& visitor, const variant<Args...>& ) {
                return visitor();
            }

            template <size_t Idx, class Visitor, class ...Args>
            static decltype(auto) apply_visitor(Visitor&& visitor, variant<Args...>&& ) {
                return visitor();
            }
        };
    }


    template<size_t I, class... Ts>
    decltype(auto) get(variant<Ts...>& v) {
        using ret_type = detail::get_type_t<I, Ts...>;
        auto ret = detail::helper<Ts...>::template get<ret_type, I>(v.buffer_, v.type_idx_);
        return ret ? *ret : throw(bad_get("no such type"));
    }

    template<size_t I, class... Ts>
    decltype(auto) get(const variant<Ts...>& v) {
        using Type = std::add_const_t<detail::get_type_t<I, Ts...>>;
        auto ret = detail::helper<Ts...>::template get<Type, I>(
                const_cast<std::byte *>(v.buffer_), v.type_idx_);
        return ret ? *ret : throw(bad_get("no such type"));
    }

    template<size_t I, class... Ts>
    decltype(auto) get(variant<Ts...>&& v) {
        return std::move(get<I, Ts...>(v));
    }


    template<size_t I, class... Ts>
    decltype(auto) get(variant<Ts...>* v) {
        using Type = detail::get_type_t<I, Ts...>;
        return detail::helper<Ts...>::template get<Type, I>(v->buffer_, v->type_idx_);
    }

    template<size_t I, class... Ts>
    decltype(auto) get(const variant<Ts...>* v) {
        using Type = std::add_const_t<detail::get_type_t<I, Ts...>>;
        return detail::helper<Ts...>::template get<Type, I>(const_cast<std::byte *>(v->buffer_), v->type_idx_);

    }

    template<class T, class ...Ts>
    T& get(variant<Ts...>& v) {
        const size_t idx = detail::get_idx<T, 0, Ts...>::value;
        auto ret = detail::helper<Ts...>::template get<T, idx>(v.buffer_, v.type_idx_);
        return ret ? *ret : throw(bad_get("no such type"));
    }

    template<class T, class ...Ts>
    const T& get(const variant<Ts...>& v) {
        const size_t idx = detail::get_idx<T, 0, Ts...>::value;
        const T* ret = detail::helper<Ts...>::template get<T, idx>(const_cast<std::byte *>(v.buffer_), v.type_idx_);
        return ret ? *ret : throw(bad_get("no such type"));
    }

    template<class T, class ...Ts>
    T&& get(variant<Ts...>&& v) {
        return std::move(get<T, Ts...>(v));
    }

    template<class T, class ...Ts>
    T* get(variant<Ts...>* v) {
        const size_t idx = detail::get_idx_v<T, Ts...>;
        return detail::helper<Ts...>::template get<T, idx>(v->buffer_, v->type_idx_);
    }

    template<class T, class ...Ts>
    const T* get(const variant<Ts...>* v) {
        const size_t idx = detail::get_idx_v<T, Ts...>;
        return detail::helper<Ts...>::template get<std::add_const_t<T>, idx>(const_cast<std::byte *>(v->buffer_), v->type_idx_);

    }

    template <class... Ts>
    class variant {
    private:
        constexpr static size_t ALIGNMENT = std::max({alignof(Ts)... });
        constexpr static size_t SIZE = std::max({sizeof(Ts)... });

    public:
        variant() = default;

        template <class U, std::enable_if_t<!std::is_same_v<std::decay_t<U>, variant>, bool> = true>
        variant(U&& obj) {
        if (!detail::helper<Ts...>::template create<U, 0>(buffer_, std::forward<U>(obj), type_idx_))
            detail::helper<Ts...>::template convert<U, 0>(buffer_, std::forward<U>(obj), type_idx_);
        }

        template <class U>
        variant& operator=(const U& obj) {
            variant tmp(obj);
            clear();
            *this = std::move(tmp);
            return *this;
        }

        ~variant() {
            if (!empty()) {
                detail::helper<Ts...>::template destroy<0>(buffer_, type_idx_);
            }
        }

        variant(const variant &other) : type_idx_(other.type_idx_) {
            detail::helper<Ts...>::template copy<0>(buffer_, other.buffer_, other.type_idx_);
        }

        variant& operator=(const variant &other) {
            if (this == &other) {
                return *this;
            }
            variant backup(std::move(*this));
            try {
                detail::helper<Ts...>::template copy<0>(buffer_, other.buffer_, other.type_idx_);
                type_idx_ = other.type_idx_;
            } catch (...) {
                *this = std::move(backup);
                throw;
            }
            return *this;
        }

        variant(variant<Ts...> &&other) noexcept((std::is_nothrow_move_constructible_v<Ts> && ...))
        : type_idx_(other.type_idx_) {
            detail::helper<Ts...>::template move<0>(buffer_, other.buffer_, other.type_idx_);
        }
        variant& operator=(variant &&other) noexcept((std::is_nothrow_move_assignable_v<Ts> && ...)) {
            if (this == &other) {
                return *this;
            }

            if (empty()) {
                if (other.empty()) {
                    return *this;
                } else {
                    swap(other);
                }
            } else {
                if (other.empty()) {
                    clear();
                } else {
                    swap(other);
                    other.clear();
                }
            }

            return *this;
        }

        void swap(variant<Ts...> &other) noexcept((std::is_nothrow_move_constructible_v<Ts> && ...)) {
            auto tmp(std::move(*this));
            auto this_t_i = other.type_idx_;
            auto other_t_i = tmp.type_idx_;
            detail::helper<Ts...>::template move<0>(buffer_, other.buffer_, other.type_idx_);
            detail::helper<Ts...>::template move<0>(other.buffer_, tmp.buffer_, tmp.type_idx_);
            type_idx_ = this_t_i;
            other.type_idx_ = other_t_i;
        }

        bool empty() const {
            return type_idx_ == util::variant_npos;
        }

        [[nodiscard]] size_t index() const {
            return type_idx_;
        }

        void clear() {
            if (empty()) {
                return;
            }
            detail::helper<Ts...>::template destroy<0>(buffer_, type_idx_);
            type_idx_ = util::variant_npos;
        }

        template<size_t I, class... Args>
        friend decltype(auto) get(variant<Args...>& v);

        template<size_t I, class... Args>
        friend decltype(auto) get(const variant<Args...>& v);

        template<size_t I, class... Args>
        friend decltype(auto) get(variant<Args...>&& v);

        template<size_t I, class... Args>
        friend decltype(auto) get(variant<Args...>* v);

        template<size_t I, class... Args>
        friend decltype(auto) get(const variant<Args...>* v);


        template<class T, class ...Args>
        friend T& get(variant<Args...>& v);

        template<class T, class ...Args>
        friend const T& get(const variant<Args...>& v);

        template<class T, class ...Args>
        friend T&& get(variant<Args...>&& v);

        template<class T, class ...Args>
        friend T* get(variant<Args...>* v);

        template<class T, class ...Args>
        friend const T* get(const variant<Args...>* v);


    private:
        alignas(ALIGNMENT) std::byte buffer_[SIZE] = {};
        size_t type_idx_ = util::variant_npos;

    };

    template<class... Args>
    void swap(variant<Args...>& v1, variant<Args...>&v2) {
        v1.swap(v2);
    }

    template <class Visitor, class... Ts>
    decltype(auto) apply_visitor(Visitor&& visitor, const variant<Ts...>& var) {
        return detail::helper<Ts...>::template apply_visitor<0, Visitor>(std::forward<Visitor>(visitor), var);
    }

    template <class Visitor, class... Ts>
    decltype(auto) apply_visitor(Visitor&& visitor, variant<Ts...>& var) {
        return detail::helper<Ts...>::template apply_visitor<0, Visitor>(std::forward<Visitor>(visitor), var);
    }

    template <class Visitor, class... Ts>
    decltype(auto) apply_visitor(Visitor&& visitor, variant<Ts...>&& var) {
        return detail::helper<Ts...>::template apply_visitor<0, Visitor>(std::forward<Visitor>(visitor), std::move(var));
    }

}
