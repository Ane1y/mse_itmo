#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <queue>
//Толик придумал новую технологию программирования. Он хочет уговорить друзей использовать ее. Однако все не так просто.
// i-й друг согласится использовать технологию Толика, если его авторитет будет не меньше a_i (авторитет выражается целым числом).
// Как только i-й друг начнет ее использовать, к авторитету Толика прибавится число b_i (попадаются люди, у которых b_i < 0).
// Помогите Толику наставить на путь истинный как можно больше своих друзей.

struct influence {
    int64_t a;
    int64_t b;
    size_t num;
};
using inf_vec = std::vector<influence>;

void print_influence_vec(const inf_vec& vec) {
    for (const auto& v : vec) {
        std::cout << v.a << " " << v.b << ", num = " << v.num + 1 << std::endl;
    }
}

void print_recruited(const inf_vec& people, const std::vector<bool>& recruited) {
    for (const auto &person : people) {
        if (recruited[person.num]) {
            std::cout << person.num + 1 << " ";
        }
    }
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    size_t n = 0;
    int64_t a0 = 0;
    std::cin >> n >> a0;

    int64_t a = 0;
    int64_t b = 0;
    inf_vec p_minions;
    inf_vec n_minions;
    inf_vec minions(n);
    size_t recruited_counter = 0;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> a >> b;
        if (b < 0) {
            n_minions.push_back({a, b, i});
        } else {
            p_minions.push_back({a, b, i});
        }
        minions[i] = {a, b, i};
    }
    //обработка положительных приспешников
    std::sort(p_minions.begin(), p_minions.end(), [](const influence& p1, const influence& p2){
        return p1.a < p2.a; });
    std::vector<bool> recruited(n, false);
    for (const auto& minion : p_minions) {
        if (minion.a > a0) {
            break;
        }
        recruited[minion.num] = true;
        ++recruited_counter;
        a0 += minion.b;
    }

    //обработка отрицательных приспешников
    std::sort(n_minions.begin(), n_minions.end(),
              [](const influence& p1, const influence& p2) { return p1.b + p1.a > p2.a + p2.b;});

    auto heap_comp = [](const influence& p1, const influence& p2){return p1.b > p2.b;};
    std::priority_queue<influence, std::vector<influence>, decltype(heap_comp)> recruited_heap(heap_comp);

    for (const auto& minion : n_minions) {
        if (minion.a <= a0) {
            recruited[minion.num] = true;
            ++recruited_counter;
            recruited_heap.push(minion);
            a0 += minion.b;
        } else if (!recruited_heap.empty()) {
                auto worst = recruited_heap.top();
                if ((a0 - worst.b) >= minion.a && minion.b > worst.b) {
                    recruited[worst.num] = false;
                    recruited_heap.pop();

                    recruited_heap.push(minion);
                    recruited[minion.num] = true;

                    a0 = a0 + minion.b - worst.b;
                }
        }
    }
    //вывод друзей
    std::cout << recruited_counter << std::endl;
    print_recruited(p_minions, recruited);
    print_recruited(n_minions, recruited);

    return 0;
}



