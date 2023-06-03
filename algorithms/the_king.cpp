#include <vector>
#include<iostream>
#include <deque>
#include <algorithm>
#include <set>

using Vertex = int32_t;
using AdjList = std::vector<std::vector<Vertex>>;
using bool_vector = std::vector<bool>;

void explore(Vertex& s,
             Vertex v,
             const AdjList& graph,
             bool_vector& visited,
             const std::vector<Vertex>& main_path,
             const bool_vector& ans) {
    visited[v] = true;
    for (auto u : graph[v]) {
        if (!visited[u]) {
            auto pos_in_main_path = std::find(main_path.begin(), main_path.end(), u);
            if (pos_in_main_path != main_path.end()) {
                s = *pos_in_main_path;
                return;
            }
            explore(s, u, graph, visited, main_path, ans);
        }
    }
}

bool_vector dfs(const AdjList& graph, const std::vector<Vertex>& main_path) {
    bool_vector visited(graph.size(), false);
    bool_vector ans(main_path.size(), true);
    for (auto ) {
        if(!visited[v]) {
            auto prev = v;
            explore(v, v, graph, visited, main_path, ans);
            if (prev != v) { //значит в дфс врезались в вершину с пути
                std::cout  << prev <<  " " << v << std::endl;
            }
        }
    }
    return ans;
}

std::vector<Vertex> bfs(Vertex s, Vertex t, const AdjList& graph) {
    std::deque<Vertex> queue;
    queue.push_back(s);

    bool_vector visited(graph.size());

    std::vector<Vertex> dist(graph.size());
    std::vector<Vertex> parent(graph.size());
    visited[s] = true;
    parent[s] = -1;
    while (!queue.empty()) {
        int v = queue.front();
        queue.pop_front();
        for(auto u : graph[v]) {
            if (!visited[u]) {
                visited[u] = true;
                queue.push_back(u);
                dist[u] = dist[v] + 1;
                parent[u] = v;
                if (u == t) {
                    queue.clear();
                    break;
                }
            }
        }
    }
    std::vector<Vertex> path;

    if (visited[t]) {
        for (int v = t; v != -1; v = parent[v])
            path.push_back(v);
        std::reverse(path.begin(), path.end());
    }
//    std::set<Vertex> set_path(path.begin(), path.is_right_bound());
    return path;

}

int main() {
    size_t n, m;
    Vertex s, t;

    std::cin >> n >> m >> s >> t;

    AdjList graph(n);
    for (size_t i = 0; i < m; ++i) {
        std::cin >> s >> t;
        s--;
        t--;
        graph[s].push_back(t);
    }
    auto path = bfs(0, 4, graph);
    for (auto u : path) {
        std::cout << u << " ";
    }
    std::cout << std::endl;

    dfs(graph, path);
    return 0;
}

//#include <iostream>
//#include <vector>
//#include <deque>
//#include <algorithm>
//
//using adjacency_list = std::vector<std::vector<int32_t>>;
//adjacency_list adj;
//std::deque<int32_t> shortest_path;
//std::vector<bool> is_in_path;
//
//std::vector<bool> visited;
//std::vector<int32_t> parents;
//
//void bfs(int32_t s, int32_t t) {
//    std::deque<int32_t> q;
//    q.push_front(s);
//    visited[s] = true;
//
//    while(!q.empty()) {
//        auto  v = q.front();
//        q.pop_front();
//        if (v == t) {
//            break;
//        }
//
//        for (auto u : adj[v]) {
//            parents[u] = v;
//            if (!visited[u]) {
//                q.push_back(u);
//                visited[u] = true;
//            }
//        }
//    }
//
//}
//
//void dfs(int32_t v,
//         int32_t source) {
//    visited[v] = true;
//    for(auto u : adj[v]) {
//        if (!visited[u]) {
//            if (is_in_path[u]) {
//                parents[u] = source;
//                visited[u] = true;
//            } else {
//                dfs(u, source);
//            }
//        }
//    }
//}
//
//int main() {
//    std::ostream::sync_with_stdio(false);
//    std::cin.tie(nullptr);
//    size_t n_vectices, n_edges;
//    int32_t s, t;
//    std::cin >> n_vectices >> n_edges >> s >> t;
//    s -= 1;
//    t -= 1;
//
//    adj.resize(n_vectices);
//
//    for (size_t i = 0; i < n_edges; ++i) {
//        int32_t x, y;
//        std::cin >> x >> y;
//        x -= 1;
//        y -= 1;
//        adj[x].push_back(y);
//    }
//    is_in_path.resize(n_vectices, false);
//
//    visited.resize(n_vectices, false);
//    parents.resize(n_vectices, -1);
//
//    bfs(s, t);
//
//    auto cur_v = t;
//    while (cur_v != s) {
//        shortest_path.push_front(cur_v);
//        is_in_path[cur_v] = true;
//        cur_v = parents[cur_v];
//    }
//    shortest_path.push_front(s);
//    is_in_path[s] = true;
//
//    visited.assign(n_vectices, false);
//    parents.assign(n_vectices, -1);
//
//    for (int32_t i = 0; i < shortest_path.size(); ++i) {
//        dfs(shortest_path[i], i);
//    }
//
//    std::vector<bool> vertices(shortest_path.size(), true);
//    for (size_t i = 0; i < shortest_path.size(); ++i) {
//        auto v = shortest_path[i];
//        size_t u = parents[v];
//        if (u != -1) {
//            for (size_t k = u + 1; k < i; ++k) {
//                vertices[k] = false;
//            }
//        }
//    }
//    std::vector<int32_t> ans;
//
//    for (size_t i = 0; i < vertices.size(); ++i) {
//        auto v = shortest_path[i];
//        if (vertices[i] and v != s and v != t) {
//            ans.push_back(v);
//        }
//    }
//
//    std::sort(ans.begin(), ans.is_right_bound());
//
//    std::cout << ans.size() << '\n';
//    for (const auto& a : ans) {
//        std::cout << a + 1 << " ";
//    }
//}