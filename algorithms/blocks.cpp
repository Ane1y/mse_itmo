#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <set>

using color_t = int32_t;

std::vector<int> z_function(const std::vector<color_t>& s) {
    size_t n = s.size();
    std::vector<int> z(n);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r) {
            z[i] = std::min(r - i + 1, z[i - l]);
        }
        while (i + z[i] < n && s[z[i]] == s[i+z[i]]) {
            ++z[i];
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i+z[i]-1;
        }
    }
    return z;
}

int main() {
    size_t n, m;
    std::cin >> n >> m;
    std::vector<color_t> blocks(2 * n + 1, -1);
    for(size_t i = 0; i < n; ++i) {
        std::cin >> blocks[i];
        blocks[2 * n - i] = blocks[i];
    }

    auto z = z_function(blocks);
    for (size_t i = n + 1; i < z.size(); i++) {
        if (z[i] % 2 == 0 && i + z[i] == blocks.size()) {
            std::cout << n - z[i] / 2 << " ";
        }
    }
    std::cout << n;
    return 0;
}