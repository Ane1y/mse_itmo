#include <cstddef>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>

class Matrix {
public:
    Matrix(size_t rows, size_t cols, int value);
    Matrix(size_t rows, size_t cols);

    friend std::ostream& operator <<(std::ostream&, const Matrix&);

    int& get_element(size_t i, size_t j);
    const int &get_element(size_t i, size_t j) const;

    static Matrix read_matrix_from_file(size_t n);

private:
    size_t rows_ = 0;
    size_t cols_ = 0;
    std::vector <int> data_;
};



Matrix::Matrix(size_t rows, size_t cols, int value) :
        Matrix(rows, cols) {
    for (size_t i = 0; i < cols_ * rows_; ++i) {
        data_[i] = value;
    }
}

Matrix::Matrix(size_t rows, size_t cols) :
        rows_(rows),
        cols_(cols),
        data_(cols_ * rows_)
{}



int &Matrix::get_element(size_t i, size_t j) {
    return data_[i * cols_ + j];
}

const int &Matrix::get_element(size_t i, size_t j) const {
    return data_[i * cols_ + j];
}

Matrix Matrix::read_matrix_from_file(size_t n) {

    Matrix matrix = Matrix(n, n, 0);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            std::cin >> matrix.get_element(i, j);
        }
    }
    return matrix;
}

std::ostream &operator<<(std::ostream &out, const Matrix &matrix) {
    for (size_t i = 0; i < matrix.rows_; ++i) {
        for (size_t j = 0; j < matrix.cols_; ++j) {
            out << matrix.get_element(i, j) << " ";
        }
        std::cout << std::endl;
    }
    return out;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    size_t n, l;
    std::cin >> n >> l;
    Matrix matrix = Matrix::read_matrix_from_file(n);

    if (l == 1) {
        std::cout << matrix;
        return 0;
    }

    const size_t windowed_size = n - l + 1;
    Matrix rows_min = Matrix(n, windowed_size);
    Matrix cols_min = Matrix(windowed_size, windowed_size);
    std::deque<int> mins;

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < l; ++j) { // первый цикл по первому окну
            while (!mins.empty() && mins.back() > matrix.get_element(i, j)) { //проинициализировали стек первыми l значениями
                mins.pop_back();
            }
            mins.push_back(matrix.get_element(i, j));
        }
        rows_min.get_element(i, 0) = mins[0];
        if (mins[0] == matrix.get_element(i, 0)) {
            mins.pop_front();
        }
        for (size_t j = l; j < n; ++j) {
            while (!mins.empty() && mins.back() > matrix.get_element(i, j) ) { //проинициализировали стек первыми l значениями
                mins.pop_back();
            }
            mins.push_back(matrix.get_element(i, j));

            rows_min.get_element(i, j - l + 1) = mins[0];
            if (mins[0] == matrix.get_element(i, j - l + 1)) {
                mins.pop_front();
            }
        }
        mins.clear();
    }


    for (size_t j = 0; j < windowed_size; ++j) { //столбцы
        for (size_t i = 0; i < l; ++i) { // первый цикл по первому окну
            while (!mins.empty() && mins.back() > rows_min.get_element(i, j)) { //проинициализировали стек первыми l значениями
                mins.pop_back();
            }
            mins.push_back(rows_min.get_element(i, j));
        }
        cols_min.get_element(0, j) = mins[0];
        if (mins[0] == rows_min.get_element(0, j)) {
            mins.pop_front();
        }
        for (size_t i = l; i < n; ++i) {
            while (!mins.empty() && mins.back() > rows_min.get_element(i, j)) { //проинициализировали стек первыми l значениями
                mins.pop_back();
            }
            mins.push_back(rows_min.get_element(i, j));

            cols_min.get_element(i - l + 1, j) = mins[0];
            if (mins[0] == rows_min.get_element(i - l + 1, j)) {
                mins.pop_front();
            }
        }
        mins.clear();

    }
    std::cout << cols_min;

    return 0;
}
