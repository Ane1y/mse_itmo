#include <vector>
#include <string>
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

std::string concat_string(const std::string& s, const std::string& t) {
    std::string r = t + "щ" + s;
    auto z = z_function(r);
    size_t to_delete = 0;
    for (int64_t i = t.size(); i < z.size(); ++i) {
        if (z[i] + i == r.length()) {
            to_delete = std::max(to_delete, z[i]);
        }
    }
    return t.substr(to_delete);

}
int main() {
    std::string s, t;
    std::cin >> s;
    std::cin >> t;

    std::string glued = concat_string(s, t);
    auto z = z_function(glued);
    size_t idx = 0;
    for(size_t i = 0; i < glued.size(); i++) {
        if (i + z[i] == glued.size()) {
            idx = i;
            break;
        }
    }

    std::string glued2 = concat_string(t, s);
    size_t idx2 = 0;
    z = z_function(glued2);
    for(size_t i = 0; i < glued2.size(); i++) {
        if (i + z[i] == glued2.size()) {
            idx2 = i;
            break;
        }
    }
    std::cout << (idx > idx2 ? glued.substr(0, idx) : glued2.substr(0, idx2));

    if (idx == 0 and idx2 == 0) {
        std::cout << glued;
    }
    return 0;
}
