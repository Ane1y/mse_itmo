#include <iostream>
#include <cstring>
#include <fstream>
#include <stack>
#include <vector>
#include <queue>
#include <algorithm>

struct Event{
    size_t idx;
    int64_t bound;
    bool is_right_bound;
    uint16_t color;
};

int main() {
    size_t n_desks, n_colors, n_friends;
    std::cin >> n_desks >> n_colors >> n_friends;

    std::vector<Event> queries;

    for (size_t i = 1; i <= n_friends ; ++i) {
        uint16_t c;
        int64_t l, r;
        std::cin >> c >> l >> r;
        c -= 1;
        queries.push_back({i, l, false, c});
        queries.push_back({i, r, true, c});
    }

    auto comp  = [](const Event& lhs, const Event& rhs) {
        if(lhs.bound == rhs.bound && lhs.idx == rhs.idx) {
            return rhs.is_right_bound;
        }
        if (lhs.bound == rhs.bound) {
            return lhs.idx > rhs.idx;
        }
        return lhs.bound < rhs.bound;
    };
    std::sort(queries.begin(), queries.end(), comp);


    auto queue_comp =  [](const Event& lhs, const Event &rhs) { return lhs.idx < rhs.idx; };
    std::priority_queue<Event, std::vector<Event>, decltype(queue_comp)> lefts(queue_comp);

    std::vector<uint64_t> painted_count(n_colors , 0);
    std::vector<bool> processed(n_friends, false);

    int64_t cur_desk = 0;

    for(auto & query : queries) {
        if (query.is_right_bound) {
            bool is_proceed = false;
            if(!lefts.empty() && lefts.top().idx == query.idx && !processed[query.idx - 1]) {
                painted_count[lefts.top().color] += query.bound - lefts.top().bound + 1;
            }
            processed[query.idx - 1] = true;
            while(!lefts.empty() && processed[lefts.top().idx - 1]) {
                lefts.pop();
                is_proceed = true;
            }

            if(!lefts.empty() && is_proceed) {
                Event tmp = lefts.top();
                lefts.pop();
                tmp.bound = query.bound + 1;
                lefts.push(tmp);
            }
            cur_desk = query.bound;
        } else {
            if(!lefts.empty() and lefts.top().idx < query.idx) {
                painted_count[lefts.top().color] += (query.bound - lefts.top().bound < 0) ? 0 : query.bound - lefts.top().bound;
            }

            if(cur_desk == query.bound) {
                query.bound = cur_desk + 1;
            }

            lefts.push(query);
        }
    }

    for(auto p : painted_count)
        std::cout << p << " ";


    return 0;
}
