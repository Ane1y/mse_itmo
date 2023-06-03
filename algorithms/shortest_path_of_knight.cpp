#include <vector>
#include <cstddef>
#include <valarray>
#include <iostream>
#include <queue>
#include <stack>

struct turn {
    int dx;
    int dy;
};

const std::vector<turn> turns{{-2, -1}, {-2, 1},
                        {-1, -2}, {-1, 2},
                        {1, -2}, {1, 2},
                        {2, -1}, {2, 1}
                        };

const size_t board_size = 8;

size_t get_coord(const turn cur_turn) {
    return cur_turn.dy * board_size + cur_turn.dx;
}

std::vector<size_t> get_neighbours(size_t v) {
    std::vector<size_t> result;
    turn vertice{ v % board_size, v / board_size };
    for (const auto &t : turns) {
        turn tmp {vertice.dx + t.dx, vertice.dy + t.dy};
        if (tmp.dx >= 0 and tmp.dy >= 0 and tmp.dx <= 7 and tmp.dy <= 7) {
            size_t cur_v = get_coord(tmp);
            result.push_back(cur_v);
        }
    }
    return result;
}

std::vector<size_t> bfs(size_t root) {
    std::vector<bool> explored(board_size * board_size, false);
    std::vector<size_t> parents(board_size * board_size, 0);
    std::queue<size_t> q;
    q.push(root);
    explored[root] = true;
    while (!q.empty()) {
        size_t v = q.front();
        q.pop();

        std::vector<size_t> neigh = get_neighbours(v);
        for (size_t u : neigh) {
            if (!explored[u]) {
                q.push(u);
                explored[u] = true;
                parents[u] = v;
            }
        }
    }
    return parents;
}



int main() {
    std::string start, end;
    std::cin >> start >> end;
    const turn start_pos = {start[1] - '1', start[0] - 'a'};
    const turn end_pos = {end[1] - '1', end[0] - 'a'};

    std::vector<size_t> parents = bfs(get_coord(start_pos));

    size_t cur_pos = get_coord(end_pos);
    std::deque<size_t> ans;
    while(cur_pos != get_coord(start_pos)) {
        ans.push_back(cur_pos);
        cur_pos = parents[cur_pos];
    }
    ans.push_back(get_coord(start_pos));

    while (!ans.empty()) {
        auto cur_cell = ans.back();
        ans.pop_back();
        char c = cur_cell / board_size + 'a';
        size_t pos = cur_cell % board_size + 1;
        std::cout << c << pos << "\n";
    }
    return 0;
}
