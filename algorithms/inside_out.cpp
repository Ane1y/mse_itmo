#include <bitset>
#include <vector>
#include <iostream>
#include <numeric>
using namespace std;

using Row = std::bitset<201>;
using Matrix = std::vector<Row>;

bool gauss(Matrix mat, size_t n_row, size_t n_col, Row& ans) {
    vector<size_t> where(n_col, -1);
    for (size_t col = 0, row = 0; col < n_col && row < n_row; ++col) {
        for (size_t i = row; i < n_row; ++i) {
            if (mat[i][col]) {
                std::swap(mat[i], mat[row]);
                break;
            }
        }

        if (!mat[row][col]) {
            continue;
        }
        where[col] = row;

        for (size_t i = 0; i < n_row; ++i) {
            if (i != row && mat[i][col]) {
                mat[i] ^= mat[row];
            }
        }
        ++row;
    }

    for (size_t i = 0; i < n_col; ++i) {
        if (where[i] != -1) {
            ans[i] = mat[where[i]][n_col];
        }
    }
    for (size_t i = 0; i < n_row; ++i) {
        int sum = 0;
        for (size_t j = 0; j < n_col; ++j) {
            sum += ans[j] * mat[i][j];
        }
        if ((sum - mat[i][n_col]) % 2 != 0) {
            return false;
        }
    }


    return true;
}
int main() {

    size_t n;
    std::cin >> n;

    size_t dependency;
    Matrix mat(n + 1);

    for (size_t i = 0; i < n; ++i) {
        std::cin >> dependency;
        size_t col;
        for (size_t j = 0; j < dependency; ++j) {
            std::cin >> col;
            mat[col - 1][i] = true;
        }
    }

    for (size_t i = 0; i < n; ++i) {
        int c;
        std::cin >> c;
        mat[i][n] = 1 - c;
    }

    Row ans(n);
    bool solved = gauss(mat, n, n, ans);
    if (!solved) {
        for(size_t i = 0; i < n; ++i) {
            mat[i][n] = 1 - mat[i][n];
        }
        ans.reset();
        if (!gauss(mat, n, n, ans)) {
            std::cout << -1;
            return 0;
        }
    }

    size_t num = 0;
    for (size_t i = 0; i < n; i++) {
        if (ans[i]) {
            num += 1;
        }
    }
    std::cout << num << " ";
    for(size_t i = 0; i < n; ++i) {
        if (ans[i]) {
            std::cout << i + 1 << " ";
        }
    }
    return 0;
}
