#pragma once
// whatever.hpp - header-only solution goes into this file.

#include <utility>
#include <stdexcept>
#include <string>
namespace utils {

    struct HolderBase {
        virtual HolderBase* clone() const = 0;
        virtual ~HolderBase() = default;
    };

    template <class T>
    struct Holder : HolderBase {
        explicit Holder(const T& t) : obj(t) {}
        Holder * clone() const override { return new Holder(obj); }

        T obj;
    };

    class whatever {
    public:
        whatever();

        template<class T, std::enaHolderble_if_t<!std::is_same_v<whatever, std::decay_t<T>>, bool> = true >
        whatever(T &&);

        whatever(whatever const &);
        whatever& operator=(whatever const &);

        ~whatever();
        [[nodiscard]] bool empty() const {return holder_ == nullptr;}

        whatever(whatever &&) noexcept;
        whatever& operator=(whatever &&) noexcept;

        void swap(whatever &);

        void clear();
        template<class T>
        friend const T* whatever_cast(const whatever *t);

        template<class T>
        friend T* whatever_cast(whatever *t);

        template<class T>
        friend T& whatever_cast(whatever &t);

        template<class T>
        friend T whatever_cast(const whatever &t);

    private:
        HolderBase* holder_;
    };

    void swap(whatever& lhs, whatever& rhs) {
        lhs.swap(rhs);
    }


    class bad_whatever_cast : public std::runtime_error {
    public:
        bad_whatever_cast(std::string const &msg) : std::runtime_error(msg) {}
    };

    whatever::whatever() : holder_(nullptr) {}

    template<class T, std::enable_if_t<!std::is_same_v<whatever, std::decay_t<T>>, bool>>
    whatever::whatever(T&& t) : holder_(new Holder<std::decay_t<T>>{std::forward<T>(t)}) {}

    whatever::whatever(const whatever &other) : holder_(other.empty() ? nullptr : other.holder_->clone()) {}

    whatever& whatever::operator=(const whatever &other) {
        if (this == &other) {
            return *this;
        }
        whatever tmp(other);
        *this = std::move(tmp);
        return *this;
    }

    void whatever::swap(whatever &other) {
        std::swap(holder_, other.holder_);
    }

    whatever::~whatever() {
        delete holder_;
    }

    template<class T>
    const T* whatever_cast(const whatever *t) {
        if (!t) {
            return nullptr;
        }
        auto holder = dynamic_cast<Holder<std::decay_t<T>>*>(t->holder_);
        return holder ? &holder->obj : nullptr;
    }

    template<class T>
    T* whatever_cast(whatever *t) {
        if (!t) {
            return nullptr;
        }
        auto holder = dynamic_cast<Holder<std::decay_t<T>>*>(t->holder_);
        return holder ? &holder->obj : nullptr;
    }

    template <class T>
    T& whatever_cast(whatever &t) {
        auto holder = dynamic_cast<Holder<std::decay_t<T>>*>(t.holder_);
        if (holder == nullptr) {
            throw bad_whatever_cast("bad cast");
        }
        return holder->obj;
    }

    template<class T>
    T whatever_cast(const whatever &t) {
        auto holder = dynamic_cast<const Holder<std::decay_t<T>>*>(t.holder_);
        if (holder == nullptr) {
            throw bad_whatever_cast("bad cast");
        }
        return holder->obj;
    }

    whatever::whatever(whatever &&other) noexcept : holder_(std::exchange(other.holder_, nullptr)){}

    whatever &whatever::operator=(whatever &&other)  noexcept {
        std::swap(holder_, other.holder_);
        return *this;
    }

    void whatever::clear() {
        delete holder_;
        holder_ = nullptr;
    }


}
