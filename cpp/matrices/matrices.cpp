#include "matrices.hpp"

Matrix add(const Matrix& a, const Matrix& b) {
    Matrix result = {a.cols, a.rows, nullptr};
    result.data = new double*[a.cols];
    for (size_t i = 0; i < a.cols; i++) {
        result.data = new double*[a.rows];
        for (size_t j = 0; j < a.rows; j++) {
            result.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }
    return result;
}


Matrix mult(const Matrix& a, const Matrix& b) {
    Matrix result = {a.cols, a.rows, nullptr};
    result.data = new double*[a.cols];
    for(size_t i = 0; i < a.rows; ++i) {
        for (size_t j = 0; j < b.cols; ++j) {
            for (size_t k = 0; k < a.cols; ++k) {
                result.data[i][j] += a.data[i][k] * b.data[k][j];
            }
        }
    }
    return result;
}
