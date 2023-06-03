#include <algorithm>
#include <iostream>
#include <vector>

int main () {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    size_t n;
    std::cin >> n;
    std::vector<int> nums(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> nums[i];
    }
    std::sort(nums.begin(), nums.end());
    std::cout << std::endl;
    size_t k;
    long long l, r;
    std::cin >> k;
    std::cin >> l >> r;
    auto begin = nums.begin();
    auto end = nums.end();
    std::cout << std::upper_bound(begin, end, r) - std::lower_bound(begin, end, l);
    for (size_t i = 1; i < k; ++i) {
        std::cin >> l >> r;
        std::cout << " " << std::upper_bound(begin, end, r) - std::lower_bound(begin, end, l);
    }

    return 0;
}