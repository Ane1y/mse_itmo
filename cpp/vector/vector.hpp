
#pragma once
#include <new>
#include <algorithm>
#include <iostream>
#include <memory>

namespace cls01 {

    template<class T>
    class Vector {
    public:
        using size_type = std::size_t;

        Vector(size_type, const T &);
        ~Vector();

        size_type size() const noexcept;

        void resize(size_type, const T&) noexcept(false);
        void reserve(size_type);
        // Implement this method with a strong exception safety guarantee + annotate with noexcept
        void push_back(const T&);

    private:
        static T *create_storage(size_type);

        Vector(const Vector &) = delete;
        Vector(Vector &&) noexcept = delete;

        Vector &operator=(const Vector &) = delete;
        Vector &operator=(Vector &&) noexcept = delete;

        void del_storage(T*, size_t);

        T *begin_ = nullptr;
        T *end_ = nullptr;
        size_type size_ = 0;
        size_type capacity_ = 0;

    };

    template<class T>
    void Vector<T>::reserve(Vector::size_type new_cap) {
        if (capacity_ >= new_cap) {
            return;
        }

        T* new_storage = create_storage(new_cap);
        size_t i = 0;
        try {
            size_t data_size = (new_cap < size_) ? new_cap : size_;
            for (; i < data_size; ++i) {
                new(new_storage + i) T(std::move_if_noexcept(begin_[i]));
            }
            del_storage(begin_, size_);
            begin_ = new_storage;
            size_ = data_size;
            capacity_ = new_cap;

        } catch (...) {
            del_storage(new_storage, i);
            std::rethrow_exception(std::current_exception());
        }

    }

    template<class T>
    void Vector<T>::del_storage(T* begin, size_t size) {
        std::destroy(begin, begin + size);
        operator delete[](begin, std::align_val_t{alignof(T)});
    }


    template<class T>
    Vector<T>::Vector(Vector::size_type count, const T &value)
            : begin_(create_storage(count))
            , end_(begin_ + count)
            , capacity_(count) {
        for (size_type i = 0; i < count; ++i) {
            push_back(value);
        }
    }


    template<class T>
    Vector<T>::~Vector() {
        del_storage(begin_, size_);
    }

    template<class T>
    typename Vector<T>::size_type Vector<T>::size() const noexcept {
        return size_;
    }

    template<class T>
    void Vector<T>::resize(Vector::size_type count, const T& value) noexcept(false) {
        if (count < size_) {
            std::destroy(begin_ + count, begin_ + size_);
            size_ = count;
            return;
        }
        reserve(count);
        std::fill(begin_ + size_, begin_ + count, value);
        size_ = count;
    }

    template<class T>
    void Vector<T>::push_back(const T &value) {
        if (size_ == capacity_) {
            reserve(2 * std::max(capacity_, static_cast<size_type>(2)));
        }
        new (begin_ + size_) T(value);
        ++size_;

    }

    template<class T>
    T *Vector<T>::create_storage(Vector::size_type count) {
        void *storage = new(std::align_val_t{alignof(T)}) char[sizeof(T) * count];
        return static_cast<T*>(storage);
    }

}


