#include <iostream>
#include <vector>
#include <limits>
#include <queue>

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

    Graph(size_t size, vertex_t s, vertex_t t) : size_(size),
                                   s_(s),
                                   t_(t) {
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

int main() {
    int n, m;
    flow_t w, b, g;
    std::cin >> n >> m >> w >> b >> g;

    char color;
//    std::vector<char> colors(n * m);

    vertex_t s = n * m;
    vertex_t t = s + 1;
    Graph graph(t + 1, s, t);


    for (int i = 0; i < n * m; ++i) {
    std::cin >> color;

    if (i - m >= 0) {
        graph.add_edge(i, i - m, g);
    }
    if (i + m < n * m) {
        graph.add_edge(i, i + m, g);
    }
    if (i % m != 0) {
        graph.add_edge(i, i - 1, g);
    }
    if ((i + 1) % m != 0 and i + 1 < n * m) {
        graph.add_edge(i, i + 1, g);
    }

    if (color == 'B') {
        graph.add_edge(i, t, w);
    }
    if (color == 'W') {
        graph.add_edge(s, i, b);
    }
}

    double flow = dinitz(graph);
    std::cout << static_cast<int>(flow);
}