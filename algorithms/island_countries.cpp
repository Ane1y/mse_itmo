#include <iostream>
#include <vector>
#include <limits>
#include <queue>

using adjacency_list = std::vector<std::vector<uint32_t>>;
std::vector<uint32_t> dist;
std::vector<uint32_t> prev;

std::vector<uint32_t> dijkstra(const adjacency_list & adj, const std::vector<uint16_t>& whose_city, size_t source) {
    dist.resize(adj.size(), -1);
    prev.resize(adj.size(), std::numeric_limits<uint32_t>::max());
    dist[source] = 0;
    std::priority_queue<std::pair<uint32_t, uint32_t>,std::vector<std::pair<uint32_t,uint32_t>>, std::greater<>> heap;
    // first - distance
    // secind - vertex
    heap.emplace(0, source);

    while(!heap.empty()) {
        std::pair<uint32_t, uint32_t> p = heap.top();
        heap.pop();
        uint32_t u = p.second;
        uint32_t d = p.first;
        if(d > dist[u])
            continue;
        for(auto v : adj[u])  {
            size_t cur_dist = 0;
            if (whose_city[u] != whose_city[v]) {
                if (u % 2 != 0) { //потому что вершины считаются как v -= 1
                    cur_dist = 2;
                } else {
                    cur_dist = 1;
                }
            }

            size_t next_dist = d + cur_dist;
            if(next_dist < dist[v]) {
                dist[v] = next_dist;
                heap.emplace(next_dist, v);
                prev[v] = u;
            }
        }
    }
    return dist;
}

int main() {
    uint32_t n, m;
    std::cin >> n >> m;

    std::vector<uint16_t> whose_city(n);

    for(size_t i = 0; i < n; ++i) {
        std::cin >> whose_city[i];
    }
    uint32_t a, b;
    adjacency_list adj(n);
    for(size_t i = 0; i < m; ++i) {
        std::cin >> a >> b;
        a -= 1;
        b -= 1;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    dijkstra(adj, whose_city, 0);


    if (dist[n - 1] == -1) {
        std::cout << "impossible";
    } else {
        std::vector<uint32_t> ans;
        size_t cur = n - 1;
        while (cur != 0) {
            ans.push_back(cur + 1);
            cur = prev[cur];
        }
        ans.push_back(1);

        std::cout << dist[n  - 1] << " " << ans.size() << std::endl;
        for (int i = ans.size() - 1; i >= 0; --i) {
            std::cout << ans[i] << " ";
        }
    }

    return 0;
}