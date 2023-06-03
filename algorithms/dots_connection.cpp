//
// Created by anely on 11/25/22.
//
//Даны N точек на плоскости. Требуется провести отрезки между некоторыми парами точек таким образом, чтобы, во-первых,
//из любой данной точки в любую можно было пройти по этим отрезкам, а во-вторых, суммарная длина проведённых отрезков
//была минимальна.
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iomanip>
using Point = std::pair<int64_t, int64_t>;

struct Edge {
    size_t u;
    size_t v;
    int64_t weight;
};

int64_t dist(const Point& x, const Point& y) {
    return (x.first - y.first) * (x.first - y.first) + (x.second - y.second) * (x.second - y.second);
}

std::vector<size_t> parents;
std::vector<size_t> ranks;

void make_set(size_t v) {
    parents[v] = v;
    ranks[v] = 0;

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
using T = std::vector<std::pair<size_t, size_t>>;
using vert_vec = std::vector<std::pair<int64_t, int64_t>>;

double length = 0;

T kruskal(const vert_vec& vertices, std::vector<Edge>& edges) {
    size_t n = vertices.size();
    parents.resize(n);
    std::iota(parents.begin(), parents.end(), 0);
    ranks.resize(n, 0);
    T tree;
    std::sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2){return e1.weight < e2.weight;});
    for (const auto& edge : edges) {
        auto u = edge.u;
        auto v = edge.v;
        if (find(u) != find(v)) {
            tree.push_back({u, v});
            length += std::sqrt(static_cast<double>(edge.weight));
            union_set(u, v);
        }
    }
    return tree;
}
int main() {
    size_t n = 0;
    std::cin >> n;

    std::vector<std::pair<int64_t, int64_t>> vertices(n);

    for(size_t i = 0; i < n; ++i) {
        std::cin >> vertices[i].first >> vertices[i].second;
    }
    std::vector<Edge> edges;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i; j < n; ++j) {
            if (i != j) {
                edges.push_back({i, j, dist(vertices[i], vertices[j])});
            }
        }
    }
    T tree = kruskal(vertices, edges);

    std::cout << std::fixed << std::setprecision(6) <<  length << '\n';
    std::cout << tree.size() << '\n';
    for (const auto& t : tree) {
        std::cout << t.first + 1 << " " << t.second + 1 << '\n';
    }

    return 0;
}