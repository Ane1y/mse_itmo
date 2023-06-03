#include <iostream>
#include <cstddef>
#include <algorithm>

int main() {
    size_t n = 0;
    std::cin >> n;

    long long left = 0;
    long long bot = 0;
    long long right = 0;
    long long top = 0;
    std::cin >> left >> bot >> right >> top;

    for (size_t i = 1; i < n; ++i) {
        long long l = 0, b = 0, r = 0, t = 0;
        std::cin >> l >> b >> r >> t;

        left = std::max(left, l);
        bot = std::max(bot, b);
        right = std::min(right, r);
        top = std::min(top, t);
    }
    long long square = (top - bot) * (right - left);
    if (top - bot < 0 && right - left < 0 || square < 0)
        square = 0;

    std::cout << square;
    return 0;
}