#pragma once
// bignum.hpp - header-only solution goes into this file.

#include <vector>
#include <cmath>
#include <istream>
#include <regex>
#include <iostream>

namespace mp {
    class bignum {
    public:
//конструктор по умолчанию инициализирует нулём;
        bignum() = default;
//неявное конструирование от целого числа (uint32_t);
        bignum(uint32_t);
//явное конструирование от десятичной строки (std::string). Допускается квадратичная трудоемкость;
        explicit bignum(const std::string &);
//копирование (конструктор и оператор присваивания);
        bignum(const bignum &) = default;
        bignum& operator =(const bignum &) = default;
//явные преобразования к целому числу (uint32_t); если число не помещается в uint32_t, происходит сужение
// значения (как int в short);
        explicit operator uint32_t() const;
//возможность использования в условных выражениях (true, если не ноль);
        explicit operator bool() const;
//класс не должен позволять делать вычитание;
        bignum& operator -(const bignum &) = delete;
//функция получения десятичного строкового представления to_string. Допускается квадратичная трудоемкость;
        std::string to_string() const;
//ввод/вывод в стандартные потоки (std::ostream/std::istream), ввод/вывод аналогичен обычным целым числам в
// десятичном представлении, поддержка манипуляторов ввода вывода не требуется;
        friend std::ostream & operator <<(std::ostream &, const bignum &);
        friend std::istream & operator >>(std::istream &,  bignum &);

        //операции +, *, +=, *= (запятая здесь - всего лишь разделитель перечисления, перегружать ее не надо);
// Умножение имеет квадратичную трудоемкость.
//        bignum operator +(const bignum &) const;
//        bignum operator *(const bignum &) const;
        bignum& operator +=(const bignum &);
        bignum& operator *=(const bignum &);
        friend bignum operator +(const bignum&, const bignum&);
        friend bignum operator *(const bignum&, const bignum&);
    private:
        std::vector<uint32_t> num_;

        bignum multiply_by_uint32(uint32_t, size_t) const;
        char to_string_shift();

        };


    inline bignum::bignum(uint32_t num) {
        if (num != 0) {
            num_.push_back(num);
        }
    }

    inline bignum::bignum(const std::string &str_num) {
        size_t str_len = str_num.length();
        uint32_t digit = 0;
        bignum power_of_ten(1);

        for (size_t i = 0; i < str_num.length(); ++i) {
            digit = static_cast<uint32_t>(str_num[str_len - 1 - i]) - '0';
            *this += power_of_ten.multiply_by_uint32(digit, i);
            power_of_ten = power_of_ten.multiply_by_uint32(10, 0);
        }

    }


    inline bignum::operator bool() const {
        return !num_.empty();
    }

    inline std::string bignum::to_string() const {
        if (!*this) {
            return "0";
        }
        bignum num = *this;
        std::string ans;
        while(num) {
            ans += num.to_string_shift();
        }
        return {ans.rbegin(), ans.rend()};
    }
    inline char bignum::to_string_shift() {
        uint64_t shift = 0;
        uint64_t pow2_32 = uint64_t(1) << 32;
        for (size_t i = num_.size(); i > 0; --i) {
            uint64_t j = pow2_32 * shift + static_cast<uint64_t>(num_[i - 1]);
            num_[i - 1] = static_cast<uint32_t>(j / 10);
            shift = j % 10;
        }

        while (!num_.empty() && num_.back() == 0) {
            num_.pop_back();
        }
        return (uint8_t)shift + '0';
    }

     bignum operator+(const bignum &lhs, const bignum &rhs)  {
        uint64_t radix_sum = 0;
        bignum sum;

        const bignum &bigger = (lhs.num_.size() > rhs.num_.size()) ? lhs : rhs;
        const bignum &smaller = (lhs.num_.size() > rhs.num_.size()) ? rhs : lhs;

        for (size_t i = 0; i < smaller.num_.size(); ++i) {
            radix_sum = static_cast<uint64_t>(bigger.num_[i]) + static_cast<uint64_t>(smaller.num_[i]) + (radix_sum >> 32);
            sum.num_.push_back(static_cast<uint32_t>(radix_sum));
        }
        for (size_t i = smaller.num_.size(); i < bigger.num_.size(); ++i) {
            radix_sum = static_cast<uint64_t>(bigger.num_[i]) + (radix_sum >> 32);
            sum.num_.push_back(static_cast<uint32_t>(radix_sum));
        }
        radix_sum >>= 32;
        if (radix_sum != 0) {
            sum.num_.push_back(radix_sum);
        }
        return sum;
    }

    bignum bignum::multiply_by_uint32(uint32_t multiplier, size_t shift) const {
        bignum product;
        auto digit = std::vector<uint32_t>(shift, 0);
        uint64_t radix_product = 0;
        uint64_t mult = static_cast<uint32_t>(multiplier);
        for (uint32_t n : num_) {
            radix_product = static_cast<std::uint64_t>(n) * mult + (radix_product >> 32);
            product.num_.push_back(radix_product);
        }
        radix_product >>= 32;
        if (radix_product != 0) {
            product.num_.push_back(radix_product);
        }
        return product;
    }

    bignum operator *(const bignum &lhs, const bignum &rhs) {
        bignum product;
        bignum radix_product;

        for (size_t i = 0; i < rhs.num_.size(); ++i) { //получаю число uint32_t
            product += lhs.multiply_by_uint32(rhs.num_[i], i);
        }
        return product;
    }

    inline bignum &bignum::operator+=(const bignum &rhs) {
        *this = *this + rhs;
        return *this;
    }

    inline bignum &bignum::operator*=(const bignum &rhs) {
        *this = *this * rhs;
        return *this;
    }

    bignum::operator uint32_t() const {
        return num_[0];
    }

    std::ostream &operator<<(std::ostream &out, const bignum &num) {
        out << num.to_string();
        return out;
    }

    std::istream &operator>>(std::istream &in, bignum &num) {
        std::string str;
        in >> str;
        num = bignum(str);
        return in;
    }

    class polynomial {
    public:
//        Явный конструктор от строки формата: 5*x^3+2*x^1+6*x^0
//        (без пробельных символов, степень указана всегда, коэффициент может отсутствовать).
        polynomial(const std::string& str);
//        Функции at (константная и неконстантная) для доступа к коэффициенту
//        полинома по индексу. Если конструктором полином задан меньшей степени,
//        автоматически расширяет его до нужной степени (для неконстантной).
        uint32_t at(size_t) const;
        uint32_t &at(size_t);
//        Оператор () для вычисления полинома в точке. Перегрузка по int, uint32_t,
//        double, bignum. Результат имеет тот же тип, что и аргумент.
//        (Допускается реализация оператора с использованием шаблонов).
//        Внимание: количество умножений на значение аргумента должно быть
//        не более степени полинома, т.е. необходимо реализовать схему Горнера.
        template <typename T>
        T operator ()(const T&) const;
    private:
        std::vector<uint32_t> coeffs_;
    };

    inline polynomial::polynomial(const std::string &str) {
//        5*x^3+2*x^1+6*x^0
        //найдем наибольшую степень
        std::regex reg_expr(R"(x\^\d+)");
        size_t polynomial_degree = 0;

        for (auto i = std::sregex_iterator(str.begin(), str.end(), reg_expr); i != std::sregex_iterator(); ++i) {
            polynomial_degree = std::max(polynomial_degree, std::stoul(i->str().substr(2)));
        }
        coeffs_.resize(polynomial_degree + 1);
        //ищем коэффициенты
        reg_expr = std::regex(R"(\d+|\^(\d+))");
        uint32_t coeff = 1;
        for (auto i = std::sregex_iterator(str.begin(), str.end(), reg_expr); i != std::sregex_iterator(); ++i) {
            if (i->str()[0] == '^') {  //это степень
                coeffs_[std::stoul(i->str().substr(1))] = coeff;
                coeff = 1;
            } else {                   //иначе коэффициент
                coeff = std::stoul(i->str());
            }
        }

    }

    inline uint32_t polynomial::at(size_t i) const {
        return coeffs_.at(i);
    }

    inline uint32_t& polynomial::at(size_t i) {
        return coeffs_.at(i);
    }

    template<typename T>
    inline T polynomial::operator()(const T& x) const {
        T res = 0;
        for (auto it = coeffs_.rbegin(); it != coeffs_.rend(); ++it) {
            res = res * x + *it;
        }
        return res;
    }
}