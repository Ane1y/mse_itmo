//#include <vector>
//#include <bitset>
//#include <iostream>
////
//const size_t N = 2000;
//using Row = std::vector<unsigned char>;
//using Matrix = std::vector<Row>;
//
//void mod2(const Row& fst, Row& snd) {
//    if (snd.size_() < fst.size_()) {
//        return;
//    }
//    for(size_t i = 0; i < snd.size_(); ++i) {
//        snd[i] ^= fst[i];
//    }
//
//}
//std::vector<bool> immovable_gauss(Matrix& mat, Matrix& history) {
//    std::vector<bool> immutable(mat.size_(), false);
//    for (size_t column_idx = 0; column_idx < mat[0].size_(); ++column_idx) {
//        size_t pivot_row_idx = -1;
//        for(size_t row_idx = 0; row_idx < mat.size_(); ++row_idx) {
//            if (mat[row_idx][column_idx] == 0 or immutable[row_idx]) {
//                continue;
//            }
//            pivot_row_idx = row_idx;
//            immutable[pivot_row_idx] = true;
//            break;
//        }
//
//        if (pivot_row_idx == -1 || pivot_row_idx == mat.size_() - 1) {
//            continue;
//        }
//
//        for (size_t row_idx = pivot_row_idx + 1; row_idx < mat.size_(); ++row_idx) {
////            if(immutable[row_idx]) {
////                continue;
////            }
//            if(mat[row_idx][column_idx] == 1) {
//                mod2(mat[pivot_row_idx], mat[row_idx]);
////                mod2(history[row_idx], history[pivot_row_idx]);
//                history[row_idx][pivot_row_idx] = '1';
//            }
//        }
////        std::cout << std::endl << std::endl;
////        for (auto& row : mat) {
////            for (auto& el : row) {
////                std::cout << el << " ";
////            }
////            std::cout << std::endl;
////        }
//    }
//    return immutable;
//}
//int main() {
//    size_t m, n;
//    std::cin >> n >> m;
//
//    Matrix mat(m, Row(n));
//    Matrix history(m, Row(m, '0'));
//    char tmp = 0;
//    for (size_t i = 0; i < m; ++i) {
//        for (size_t j = 0; j < n; ++j) {
//            std::cin >> tmp;
//            mat[i][j] = tmp == '0' ? 0 : 1;
//            history[i][j] = i == j ? 1 : 0;
//        }
//    }
//    auto ind_vec = immovable_gauss(mat, history);
//    for (auto v: ind_vec) {
//        std::cout << (v ? "No" : "Yes") << "\n";
//    }
//    if (!*ind_vec.end()) {
//        auto &row_info = history[mat.size_() - 1];
//        for (size_t i = 0; i < row_info.size_(); ++i) {
//            if (row_info[i] == '1') {
//                for (size_t j = 0; j < history[i].size_(); ++j) {
//                    if (history[i][j] == '1') {
//                        row_info[j] = '1';
//                    }
//                }
//            }
//
//        }
//
//        for (size_t i = 0; i < row_info.size_() - 1; ++i) {
//            std::cout << (row_info[i]) << " ";
//        }
//    }
//
//    return 0;
//
//}

#include <vector>
#include <iostream>
#include <bitset>

const size_t N = 2000;
using Row = std::bitset<N>;
using Matrix = std::vector<Row>;


int main() {
    size_t m, n;
    std::cin >> n >> m;

    Matrix mat(m, Row());
    Matrix history(m, Row());
    char tmp = 0;
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            std::cin >> tmp;
            mat[i][j] = tmp != '0';
        }
    }
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < m; ++j) {
            history[i][j] = i == j;
        }
    }
    std::vector<bool> immutable(mat.size(), false);
    for (int column_idx = 0, row_idx = 0; column_idx < mat[0].size() && row_idx < mat.size(); ++column_idx) {
        int pivot_row = 0;
        for (; pivot_row < mat.size(); ++pivot_row) {
            if (!immutable[pivot_row] and mat[pivot_row][column_idx]) {
                immutable[pivot_row] = true;
                break;
            }
        }
        if (pivot_row == mat.size() or !mat[pivot_row][column_idx]) {
            continue;
        }

        for (int i = pivot_row + 1; i < mat.size(); ++i) {
            if (mat[i][column_idx]) {
                mat[i] ^= mat[pivot_row];
                history[i] ^= history[pivot_row];
            }
        }
        ++row_idx;
    }

    for (auto&& v: immutable) {
        std::cout << (v ? "No" : "Yes") << "\n";
    }
    if (!immutable[mat.size() - 1]) {
        auto &row_info = history[mat.size() - 1];
        for (size_t i = 0; i < m - 1; ++i) {
            std::cout << row_info[i] << " ";
        }
    }

    return 0;

}