#include <vector>
#include <iostream>
#include <algorithm>
#include <optional>
#include <numeric>
#include <cassert>

#define EPS = 1e-5
using vertex_t = int64_t;
using price_t = int16_t;

struct Price {
    size_t idx;
    price_t price;
};

template<class T = std::int64_t>
class RatioMod {
public:
    explicit RatioMod(T i) : RatioMod(i, 1) {}
    RatioMod(T num, T denum)
            : num_(num), denum_(denum)
    {
        assert(denum_ != 0);
        reduce();
    }

    RatioMod(RatioMod const& other) = default;
    RatioMod& operator=(RatioMod const&) = default;

    explicit operator bool() const {
        return num_ != 0;
    }

    RatioMod& operator+=(RatioMod const& other) {
        auto d = std::lcm(this->denum_, other.denum_);
        auto left = this->num_ * (d / this->denum_);
        auto right = other.num_ * (d / other.denum_);
        num_ = left + right;
        denum_ = d;
        reduce();
        return *this;
    }

    RatioMod operator-() const {
        return {-num_, denum_};
    }

    RatioMod& operator-=(RatioMod const& other) {
        *this += -other;
        return *this;
    }

    RatioMod& operator*=(RatioMod const& other) {
        RatioMod l{num_, other.denum_};
        RatioMod r{other.num_, denum_};
        num_ = l.num_ * r.num_;
        denum_ = l.denum_ * r.denum_;
        reduce();
        return *this;
    }

    RatioMod& operator/=(RatioMod const& other) {
        if (!*this) {
            return *this;
        }
        *this *= other.reverse();
        return *this;
    }

    RatioMod operator*(RatioMod const& other) const {
        RatioMod temp{*this};
        temp *= other;
        return temp;
    }

    RatioMod operator/(RatioMod const& other) const {
        RatioMod temp{*this};
        temp /= other;
        return temp;
    }

    RatioMod operator+(RatioMod const& other) const {
        RatioMod temp{*this};
        temp += other;
        return temp;
    }

    RatioMod operator-(RatioMod const& other) const {
        RatioMod temp{*this};
        temp -= other;
        return temp;
    }

    [[nodiscard]] RatioMod reverse() const {
        if (!*this) {
            throw std::underflow_error("Division by zero");
        }
        if (num_ < 0) {
            return RatioMod{-denum_, -num_};
        }
        return {denum_, num_};
    }

    [[nodiscard]] bool is_integer() const {
        return denum_ == 1;
    }

    [[nodiscard]] T round_to_integer() const {
        return num_ / denum_;
    }

    [[nodiscard]] double to_double() const {
        return static_cast<double>(num_) / denum_;
    }

    [[nodiscard]] bool equals(double d, double eps = 1e-10) const {
        return std::abs(this->to_double() - d) < eps;
    }

    [[nodiscard]] bool operator==(RatioMod const& other) const {
        return this->num_ == other.denum_ && this->denum_ == other.denum_;
    }

    [[nodiscard]] bool operator!=(RatioMod const& other) const {
        return !(this->operator==(other));
    }

    template <class U>
    friend bool operator<(RatioMod<U> const& lhs, RatioMod<U> const& rhs);

private:
    void reduce() {
        auto gcd = std::gcd(num_, denum_);
        num_ = num_ / gcd;
        denum_ = denum_ / gcd;
        if (denum_ < 0) {
            denum_ *= -1;
            num_ *= -1;
        }
    }

    T num_;
    T denum_;
};

template<class T>
bool operator<(RatioMod<T> const& lhs, RatioMod<T> const& rhs) {
    auto d = std::lcm(lhs.denum_, rhs.denum_);
    auto left = lhs.num_ * (d / lhs.denum_);
    auto right = rhs.num_ * (d / rhs.denum_);
    return left < right;
}

template<class T = std::int64_t>
class Row : public std::vector<RatioMod<T>> {
public:
    Row& operator*=(RatioMod<T> alpha) {
        for (auto& elem: *this) {
            elem *= alpha;
        }
        return *this;
    }

    Row& operator/=(RatioMod<T> alpha) {
        *this *= alpha.reverse();
        return *this;
    }

    Row& operator+=(Row const& other) {
        assert(this->size() == other.size());
        for (size_t i = 0; i < this->size(); ++i) {
            (*this)[i] += other[i];
        }
        return *this;
    }

    Row& operator-=(Row const& other) {
        assert(this->size() == other.size());
        for (size_t i = 0; i < this->size(); ++i) {
            (*this)[i] -= other[i];
        }
        return *this;
    }

    Row operator*(RatioMod<T> alpha) const {
        Row temp{*this};
        temp *= alpha;
        return temp;
    }

    Row operator/(RatioMod<T> alpha) const {
        Row temp{*this};
        temp /= alpha;
        return temp;
    }

    Row operator+(Row const& other) const {
        Row temp{*this};
        temp += other;
        return temp;
    }

    Row operator-(Row const& other) const {
        Row temp{*this};
        temp -= other;
        return temp;
    }
    [[nodiscard]] bool is_zero() const {
        bool is_zero = true;
        for(auto& el : *this) {
            if (el) {
                is_zero = false;
            }
        }
        return is_zero;
    }
};

using Matrix = std::vector<Row<vertex_t>>;

size_t find_max_pivot(const Matrix& mat, size_t row_idx, size_t column_idx) {
    size_t max_idx = row_idx;
    for (size_t i = row_idx; i < mat.size(); ++i) {
        if (std::abs(mat[i][column_idx].to_double()) > std::abs(mat[max_idx][column_idx].to_double())) {
            max_idx = i;
        }
    }
    return max_idx;
}

bool is_linear_independent(Matrix mat, size_t row_size, size_t column_size) {
    if (mat.size() == 1) {
        return true;
    }
    column_size = std::min(column_size, row_size);
    size_t virtual_column_idx = 0;
    for (size_t column_idx = 0; column_idx < column_size; ++column_idx) {
        //partial pivoting
        size_t max_row_idx = find_max_pivot(mat, column_idx, column_idx);
        if (!mat[max_row_idx][column_idx]) {
            continue;
        } else {
            virtual_column_idx += 1;
        }
        if (max_row_idx != column_idx) {
            mat[max_row_idx].swap(mat[column_idx]);
        }
        //делим на mat[row_idx, column_idx], чтобы pivot был равен единичке
        mat[column_idx] = mat[column_idx] / mat[column_idx][column_idx];
        for(size_t row_idx = virtual_column_idx; row_idx < row_size; ++row_idx) {
            if (!mat[row_idx][column_idx]) {
                continue;
            }
            Row<vertex_t> tmp = mat[column_idx];
            tmp = tmp * mat[row_idx][column_idx];
            mat[row_idx] = tmp - mat[row_idx];
        }
    }
    return !mat[mat.size() - 1].is_zero();
}

int main() {
    size_t m, n;
    std::cin >> m >> n;

    Matrix vectors;
    for (size_t i = 0; i < m; ++i) {
        Row<vertex_t> tmp;
        vertex_t value;
        for (size_t j = 0; j < n; ++j) {
            std::cin >> value;
            tmp.push_back(RatioMod(value));
        }
        vectors.push_back(tmp);
    }

    std::vector<Price> price(m);
    for (size_t i = 0; i < m; ++i) {
        std::cin >> price[i].price;
        price[i].idx = i;
    }

    std::sort(price.begin(), price.end(), [](const Price& lhs, const Price& rhs) {
        return lhs.price == rhs.price ? lhs.idx < rhs.idx : lhs.price < rhs.price;
    });

    Matrix ans;
    std::vector<size_t> n_ans;
    uint64_t total_cost = 0;
    bool has_zero_str = false;
    for (size_t i = 0; i < m; ++i) {
        Row<vertex_t>& cur_row = vectors[price[i].idx];
        ans.push_back(cur_row);
        if (!is_linear_independent(ans, ans.size(), n)) {
            ans.pop_back();
        } else {
            total_cost += price[i].price;
            n_ans.push_back(price[i].idx);
        }
        if (ans.size() == n) {
            break;
        }
    }
    std::sort(n_ans.begin(), n_ans.end());
    if (ans.size() == n) {
        std::cout << std::endl << total_cost << std::endl;
        for (auto num : n_ans) {
            std::cout << num + 1 << "\n";
        }
    } else {
        std::cout << "0";
    }
    return 0;
}
