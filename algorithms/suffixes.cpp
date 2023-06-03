#include <string>
#include <vector>
#include <iostream>


std::vector<int> p_function(const std::string& s) {
    std::vector<int> p(s.size(), 0);
    p[0] = 0;
    for (size_t i = 1; i < s.size(); ++i) {
        int k = p[i - 1];
        while (k > 0 and s[i] != s[k]) {
            k = p[k - 1];
        }
        if (s[i] == s[k]) {
            k++;
        }
        p[i] = k;
    }
    return p;
}

std::vector<size_t> z_function(const std::string& s) {
    size_t n = s.length();
    std::vector<size_t> z(n);
    for (size_t i = 1, l = 0, r = 0; i < n; ++i) {
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
    std::string str;
    std::cin>>str;

    int32_t n = str.size();

    str = str + str;

    auto z = z_function(str);

    int32_t place = 0;

    for(size_t i = 0; i < n; ++i) {
        if(z[i] > n) {
            continue;
        }
        if(str[z[i]] > str[z[i] + i]) {
            ++place;
        }
    }

    std::cout << place + 1;
}