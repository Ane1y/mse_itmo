#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iomanip>


struct Point {
    int64_t x;
    int64_t y;
    bool truly;
};
struct Edge {
    size_t u;
    size_t v;
    int64_t weight;
};

int64_t dist(const Point& x, const Point& y) {
    return (x.x - y.x) * (x.x - y.x) + (x.y - y.y) * (x.y - y.y);
}

std::vector<size_t> parents;
std::vector<size_t> ranks;

void make_set(size_t n) {
    parents.resize(n);
    std::iota(parents.begin(), parents.end(), 0);
    ranks.resize(n, 0);
}

size_t find(size_t v) {
    while (parents[v] != v){
        v = parents[v];
    }
    return v;
}
void union_set(size_t u, size_t v) {
    size_t r_u = find(u);
    size_t r_v = find(v);
    if (r_u == r_v) {
        return;
    } else if (ranks[r_u] > ranks[r_v]) {
        parents[r_v] = r_u;
    } else if (ranks[r_u] < ranks[r_v]) {
        parents[r_u] = r_v;
    } else {
        parents[r_v] = r_u;
        ranks[r_u] += 1;
    }
}

using T = std::vector<int64_t>;
using vert_vec = std::vector<Point>;


T kruskal(const vert_vec& vertices, std::vector<Edge>& edges) {
    const size_t n = vertices.size() + 2;

    make_set(n);

    T tree(n * n, -1);

    std::sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2){return e1.weight < e2.weight;});
    for (const auto& edge : edges) {
        auto u = edge.u;
        auto v = edge.v;
        if (find(u) != find(v)) {
            tree[u + v * n] = edge.weight;
            tree[v + u * n] = edge.weight;

            union_set(u, v);
        }
    }
    return tree;
}

int64_t dfs(size_t source, size_t dest, T tree, size_t n) {
    if (source == dest)
        return 0;

    for (size_t i = 0; i < n; ++i) {
        int64_t edge = tree[i + source * n];
        if (edge != -1) {
            tree[source + i * n] = tree[i + source * n] = -1;
            int64_t ans = dfs(i, dest, tree, n);
            if (ans != -1) {
                return std::max(ans, edge);
            }
        }
    }
    return -1;
}

int main() {
    size_t n = 0;
    int64_t h = 0;
    std::cin >> h >> n;
    if (n == 0) {
        std::cout << std::fixed << std::setprecision(6) <<  static_cast<double>(h);
        return 0;
    }
    std::vector<Point> coord;
    Point p;
    for(size_t i = 0; i < n; ++i) {
        std::cin >> p.x >> p.y;
        coord.push_back({p.x, p.y, true});
    }
//    for (const auto &c:coord) {
//        std::cout << c.x << " " << c.y << " "<< c.truly << std::endl;
//    }

    std::vector<Edge> edges;
    for (size_t i = 0; i < coord.size(); ++i) {
        for (size_t j = i; j < coord.size(); ++j) {
            if (i != j) {
                edges.push_back({i, j, dist(coord[i], coord[j])});
            }
        }
    }
    for (size_t j = 0; j < n; ++j) {
        edges.push_back({n, j, coord[j].y * coord[j].y});
    }

    for (size_t j = 0; j < n; ++j) {
        edges.push_back({n + 1, j, (h - coord[j].y) * (h - coord[j].y)});
    }

    T tree = kruskal(coord, edges);
    auto ans = dfs(n, n + 1, tree, n + 2);
    std::cout << std::fixed << std::setprecision(6) << std::sqrt(ans);

    return 0;
}
