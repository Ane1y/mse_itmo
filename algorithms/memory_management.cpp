#include <iostream>
#include <vector>
#include <queue>
#include <numeric>
#include <algorithm>

using min_heap = std::priority_queue<uint32_t, std::vector<uint32_t>, std::greater<>>;

struct record {
    uint32_t cell;
    uint32_t time;
};

void remove_old_cells(std::vector<uint32_t> &times, min_heap &free_memory, std::deque<record> &allocated_memory, uint32_t cur_time) {
    while (!allocated_memory.empty() && cur_time >= allocated_memory[0].time) {
        record rec = allocated_memory[0];
        allocated_memory.pop_front();
        free_memory.push(rec.cell);
    }
}
char update_time(std::vector<uint32_t> &times, min_heap &free_memory, std::deque<record> & allocated_memory, uint32_t cell, uint32_t cur_time) {
    if (cur_time >= times[cell]) {
        return '-';
    }
    auto left = std::lower_bound(allocated_memory.begin(), allocated_memory.end(), record{cell, times[cell]}, [](record lhs, record rhs) {return lhs.time < rhs.time;});
    auto to_delete = std::find_if(left, allocated_memory.end(), [& cell](const record &r){ return cell == r.cell;});
    allocated_memory.erase(to_delete);
    times[cell] = cur_time + 600;
    allocated_memory.push_back({cell, times[cell]});
    return '+';
}

int main() {
//    std::ios_base::sync_with_stdio(false);
//    std::cin.tie(nullptr);
//    std::cout.tie(nullptr);
    uint32_t cur_time, block;
    char op;
    const uint32_t time_step = 600;                                                                //10 минут из условия - каждый запрос занимает кусок памяти на 10 мин
    std::vector<uint32_t> times(30001, 0);
    //делаю странные вещи
    std::vector<uint32_t> nums(30000);
    std::iota(nums.begin(), nums.end(),  1);
    min_heap free_memory(nums.begin(), nums.end());
    std::deque<record> allocated_memory;

    while (std::cin >> cur_time) {
        std::cin >> op;
        remove_old_cells(times, free_memory, allocated_memory, cur_time);
        if (op == '.') {
            std::cin >> block;
            std::cout << update_time(times, free_memory, allocated_memory, block, cur_time) << '\n';
        } else {                                                                            //запрос на выделение памяти: выделить самый ближний блок
            uint32_t cell = free_memory.top();
            free_memory.pop();
            std::cout << cell << '\n';
            times[cell] = cur_time + time_step;
            allocated_memory.push_back({cell, times[cell]});
        }
    }
    return 0;
}
