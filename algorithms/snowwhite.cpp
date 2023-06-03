//«Ну не гномы, а наказание какое-то!», — подумала Белоснежка, в очередной раз пытаясь уложить гномов спать.
// Одного уложишь — другой уже проснулся! И так всю ночь.
//
//У Белоснежки n
//        гномов, и все они очень разные. Она знает, что для того, чтобы уложить спать i-го гнома нужно ai минут,
//        и после этого он будет спать ровно bi
//
//минут. Помогите Белоснежке узнать, может ли она получить хотя бы минутку отдыха, когда все гномы будут спать, и если
// да, то в каком порядке для этого нужно укладывать гномов спать.
//
//Например, пусть есть всего два гнома, a1=1
//, b1=10, a2=10, b2=20
//
//        . Если Белоснежка сначала начнет укладывать первого гнома, то потом ей потребуется целых 10 минут, чтобы
//        уложить второго, а за это время проснется первый. Если же она начнет со второго гнома, то затем она успеет
//        уложить первого и получит целых 10 минут отдыха.


#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>

int main() {
    size_t n = 0;
    std::cin >> n;

    auto read_data = [&](std::vector<int64_t> &v){
        for(size_t i = 0; i < n; ++i) {
            std::cin >> v[i];
        }
    };
    std::vector<int64_t> a(n);
    std::vector<int64_t> b(n);

    read_data(a);
    read_data(b);

    std::vector<size_t> d_idx(n);
    std::iota(d_idx.begin(), d_idx.end(), 0);
    std::sort(d_idx.begin(), d_idx.end(), [&](size_t i, size_t j){return a[i] + b[i] > a[j] + b[j];});

//    for (const auto& idx : d_idx) {
//        std::cout << idx <<" "<< a[idx] + b[idx] << std::endl;
//    }

    int64_t time = a[d_idx[0]];
    int64_t deadline = a[d_idx[0]] + b[d_idx[0]];
    std::vector<size_t> order(n);
    order[0] = d_idx[0];
    for (size_t j = 1; j < n; ++j) {
        size_t i = d_idx[j];
        time += a[i];
        if (time >= deadline) {
            std::cout << -1;
            return 0;
        }
        deadline = std::min(deadline, time + b[i]);
        order[j] = i;
    }
    for (auto i : order) {
        std::cout << i + 1 << " ";
    }
//    for (size_t i = 1; i < n; ++i) {
//        time += a[i]
//
//    }
    return 0;
}