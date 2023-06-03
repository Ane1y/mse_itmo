#pragma once

#include <cstddef>

struct Matrix {
    size_t cols = 0;
    size_t rows = 0;
    double** data = nullptr;
};

Matrix add(const Matrix& a, const Matrix& b);

Matrix mult(const Matrix& a, const Matrix& b);
