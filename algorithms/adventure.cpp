#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>
#include <numeric>

struct person {
    int64_t h;
    int64_t l;
};
int main() {
    size_t n = 0;
    std::cin >> n;

    std::vector<person> children(n);
    int64_t total_height = 0;

    for (size_t i = 0; i < n; ++i) {
        std::cin >> children[i].h >> children[i].l;
        total_height += children[i].h;
    }
    int64_t pit_depth = 0;
    std::cin >> pit_depth;

    std::vector<size_t> sorted_childrens_idx(n);
    std::iota(sorted_childrens_idx.begin(), sorted_childrens_idx.end(), 0);
    std::sort(sorted_childrens_idx.begin(), sorted_childrens_idx.end(), [&](size_t i, size_t j){
        auto c1 = children[i];
        auto c2 = children[j];
        return c1.h + c1.l < c2.h + c2.l;});

//    for (auto i : sorted_childrens_idx) {
//        std::cout << children[i].h << " " << children[i].l << std::endl;
//    }


    auto heap_comp = [&](size_t i, size_t j){return children[i].h < children[j].h;};
    std::priority_queue<size_t, std::vector<size_t>, decltype(heap_comp)> children_idx_heap(heap_comp);
    size_t saved_count = 0;
    std::vector<bool> saved(n, false);

    for (size_t i : sorted_childrens_idx) {
        auto child = children[i];
        //если ребенок может выбраться
        if (total_height + child.l >= pit_depth) {
            total_height -= child.h;
            children_idx_heap.push(i);
            saved[i] = true;
            ++saved_count;
        } else if (!children_idx_heap.empty()) {
            size_t j = children_idx_heap.top();
            person saved_child = children[j];
            if (child.h < saved_child.h) { //может быть среди выбравшихся есть ребенок выше?
                children_idx_heap.pop();
                total_height -= child.h;
                saved[j] = false;

                children_idx_heap.push(i);
                total_height += saved_child.h;
                saved[i] = true;
            }
        }
    }
    std::cout << saved_count << std::endl;
    for (size_t i : sorted_childrens_idx) {
        if (saved[i]) {
            std::cout << i + 1 << " ";
        }
    }

    return 0;
}
