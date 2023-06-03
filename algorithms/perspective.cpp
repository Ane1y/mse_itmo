#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using point_t = int16_t;
using minute_t = int16_t;

using vertex_t = size_t;
using flow_t = int32_t;
class Graph {
public:
    struct Edge {
        flow_t get_cur_capacity() const {
            return capacity - flow;
        }
        vertex_t to;
        vertex_t rev_idx;
        flow_t capacity;
        flow_t flow = 0;
    };

    Graph(size_t size) : size_(size + 2),
                                   s_(size),
                                   t_(size + 1) {
        adj_list.resize(size_);
    }

    void add_edge(vertex_t from, vertex_t to, flow_t capacity = 1) {
        // create edge in the Graph
        adj_list[from].push_back({to, adj_list[to].size(), capacity});
        // create edge in the residual Graph with capacity = 0
        adj_list[to].push_back({from, adj_list[from].size() - 1, 0});
    }

    Edge& get_reverse(vertex_t from, vertex_t index) {
        Edge e = adj_list[from][index];
        return adj_list[e.to][e.rev_idx];
    }

    size_t size_;
    vertex_t s_;
    vertex_t t_;
    std::vector<std::vector<Edge>> adj_list;
};


std::vector<vertex_t> bfs(Graph& graph, flow_t capacity) {
    std::vector<vertex_t> level(graph.size_, std::numeric_limits<vertex_t>::max());
    level[graph.s_] = 0;

    std::queue<vertex_t> q;
    q.push(graph.s_);

    while (!q.empty()) {
        vertex_t v = q.front();
        q.pop();

        for (const auto& e: graph.adj_list[v]) {
            if (level[e.to] == std::numeric_limits<vertex_t>::max() && e.get_cur_capacity() >= capacity) {
                level[e.to] = level[v] + 1;
                q.push(e.to);
            }
        }
    }

    return level[graph.t_] != std::numeric_limits<vertex_t>::max() ? level : std::vector<vertex_t>();
}

flow_t dinitz_dfs(Graph& graph, vertex_t v, std::vector<vertex_t>& block, std::vector<vertex_t>& leveled_network,
                  flow_t min_cap = std::numeric_limits<flow_t>::max()) {
    if (v == graph.t_) {
        return min_cap;
    }
    if (min_cap == 0) {
        return 0;
    }

    for (vertex_t i = block[v]; i < graph.adj_list[v].size(); ++i) {
        Graph::Edge& e = graph.adj_list[v][i];

        if (leveled_network[e.to] != leveled_network[v] + 1) {
            continue;
        }

        flow_t flow = dinitz_dfs(graph, e.to, block, leveled_network, std::min(e.get_cur_capacity(), min_cap));
        if (flow) {
            e.flow += flow;
            graph.get_reverse(v, i).flow -= flow;
            return flow;
        }
        ++block[v];
    }

    return 0;
}


double dinitz_step(Graph& graph, flow_t capacity) {
    double ans = 0;
    auto layered_network = bfs(graph, capacity);

    while (!layered_network.empty()) {
        std::vector<vertex_t> block(graph.size_, 0);
        double flow = dinitz_dfs(graph, graph.s_, block, layered_network);
        while (flow != 0) {
            ans += flow;
            flow = dinitz_dfs(graph, graph.s_, block, layered_network);
        }
        layered_network = bfs(graph, capacity);
    }

    return ans;
}

double dinitz(Graph& graph) {
    flow_t capacity = std::numeric_limits<flow_t>::max();
    double answer = 0;
    while (capacity != 0) {
        answer += dinitz_step(graph, capacity);
        capacity >>= 1;
    }

    return answer;
}
struct Point {
    point_t x;
    point_t y;
};

minute_t get_duration(const Point& a, const Point& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}
struct Order {
    minute_t time;
    Point departure;
    Point arrival;
    [[nodiscard]] minute_t duration() const {
        return get_duration(departure, arrival);
    }
};


using game_t = std::vector<flow_t>;
int main() {
    size_t n;
    std::cin >> n;

    game_t wins(n);
    game_t future_games(n);

    std::vector<game_t> table(n, game_t(n, 0));

    for (size_t i = 0; i < n; ++i) {
        std::cin >> wins[i];
    }

    for(size_t i = 0; i < n; ++i) {
        std::cin >> future_games[i];
    }

    wins[0] += future_games[0];

    for (auto&& w : wins) {
        if (wins[0] - w < 0) {
            std::cout << "NO";
            return 0;
        }
    }

    for (int i = 0; i < table.size(); ++i) {
        for (int j = 0; j < table[i].size(); ++j) {
            std::cin >> table[i][j];
        }
    }

    size_t graph_size = n + n * n;
    Graph graph(graph_size);

    size_t extra = n;
    flow_t plays = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 1; j < i; ++j) {
            auto& c = table[i][j];
            if (c > 0) {
                graph.add_edge(graph.s_, extra, c);
                graph.add_edge(extra, i, c);
                graph.add_edge(extra, j, c);
                extra++;
                plays += c;
            }
        }
    }

    for(size_t i = 0; i < n; ++i) {
        graph.add_edge(i, graph.t_, wins[0] - wins[i]);
    }
    auto flow = dinitz(graph);

    std::cout << (flow == plays ? "YES" : "NO");
}
