#include <vector>
#include <iostream>
#include <algorithm>

bool check(std::vector<int> &vec, size_t k, size_t d) {
    size_t count = 0;

    for (int i = vec.size() - 1; i >= 0; --i) {
        count += vec[i] / d;
        if (count >= k) {
            return true;
        }
    }
    return false;
}

size_t bin_search(std::vector<int> &vec, size_t k) {
    size_t answer = 0;
    size_t l = 1;
    size_t r = vec.back() + 1;
    while (l < r) {
        size_t d = l + (r - l) / 2;
        if (check(vec, k, d)) {
            answer = d;
            l = d + 1;
        } else {
            r = d;
        }
    }
    return answer;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t n, k;
    std::cin >> n >> k;
    std::vector<int> ropes(n);

    int sum = 0;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> ropes[i];
        sum += ropes[i];
    }
    if (sum / k < 1) {
        std::cout << 0;
        return 0;
    }
    std::sort(ropes.begin(), ropes.end());
    std::cout << bin_search(ropes, k);

}
