#include <vector>
#include <string>
#include <iostream>
#include <algorithm>


std::vector<int32_t> z_function(const std::string& s) {
    int32_t n = s.length();
    std::vector<int32_t> z(n);
    for (int32_t i = 1, l = 0, r = 0; i < n; ++i) {
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

struct op {
    int32_t type = 1;
    size_t start = 0;
    size_t index = 0;
    int32_t max_suf = 0;
};

int main() {
    std::string s;
    std::cin >> s;

    std::vector<int32_t> bytes(s.length());
    bytes[0] = 1;

    std::vector<op> history;
    history.push_back({});

    for(size_t i = 1; i < s.length(); ++i) {
        size_t idx = -1;
        int32_t max_suf = -1;
        std::string cur_prefix = s.substr(0, i + 1);
        std::reverse(cur_prefix.begin(), cur_prefix.end());
        auto z = z_function(cur_prefix);
        for(size_t j = 0; j < z.size(); ++j) {
            if(z[j] > max_suf) {
                max_suf = z[j];
                idx = j;
            }
        }
        bytes[i] = bytes[i - 1] + 1;
        history.push_back({1, i, 0, 0});
        if (bytes[i] > bytes[i - max_suf] + 5) {
            bytes[i] = bytes[i - max_suf] + 5;
            history[history.size() - 1] = {2, i - max_suf, cur_prefix.size() - (idx + max_suf) + 1,  max_suf};
        }
    }

    std::vector<op> ans;
    size_t count = 0;

    for(int64_t i = history.size() - 1; i >= 0; i--) {
        ans.push_back(history[i]);
        count++;
        if (history[i].type == 2) {
            i = history[i].start + 1;
        }
    }

    std::cout<< bytes.back() <<" "<< count << std::endl;

    for(int64_t i = ans.size() - 1; i >= 0; i--) {
        if(ans[i].type == 1) {
            std::cout<< s[ans[i].start] << std::endl;
        } else {
            std::cout << ans[i].index<<" "<< ans[i].max_suf <<std::endl;
        }
    }
}
