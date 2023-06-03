#include <iostream>
#include <vector>
#include <algorithm>

struct stats {
    long long city;
    int npeople;

    bool operator <(const stats b) const {
        if (npeople == b.npeople)
            return city < b.city;
        return npeople < b.npeople;
    }
};

int main () {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    size_t n;
    std::cin >> n;
    std::vector<stats> statistic(n);
    for (long long i = 0; i < n; ++i) {
        std::cin >> statistic[i].npeople;
        statistic[i].city = i + 1;
    }
    std::sort(statistic.begin(), statistic.end());
    size_t q;
    int l, r, x;
    std::cin >> q;

    for (size_t i = 0; i < q; ++i) {
        std::cin >> l >> r >> x;
        if (std::upper_bound(statistic.begin(), statistic.end(), stats{r, x}) > std::lower_bound(statistic.begin(), statistic.end(), stats{l, x})) {
            std::cout << 1;
        } else {
            std::cout << 0;
        }
    }
    return 0;
}