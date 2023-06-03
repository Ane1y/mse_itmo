   #include <iostream>
#include <vector>

using color = uint32_t;
using component = int32_t;
using pin = int32_t;
using wire = int32_t;
using adj_list = std::vector<std::vector<wire>>;

pin bro(wire wire, pin n_pin, const std::vector<std::pair<pin, pin>>& w2p) {
    auto [p1, p2] = w2p[wire];
    return (n_pin == p1) ? p2 : p1;
}

void dfs1(pin v, const adj_list& graph, std::vector<bool>& visited, std::vector<pin>& order) {
    visited[v] = true;
    for (auto u : graph[v]) {
        if (!visited[u])
            dfs1(u, graph, visited, order);
    }
    order.push_back(v);
}

void dfs2(pin v, component comp, const adj_list& reversed_graph, std::vector<component>& components) {
    components[v] = comp;
    for (auto u : reversed_graph[v]) {
        if (components[u] == -1) {
            dfs2(u, comp, reversed_graph, components);
        }
    }
}

std::vector<component> find_components(const adj_list& graph, const adj_list& reversed_graph) {
    std::vector<bool> visited(graph.size(), false);
    std::vector<pin> order;

    for (pin i = 0; i < graph.size(); ++i) {
        if (!visited[i]) {
            dfs1(i, graph, visited, order);
        }
    }

    std::vector<component> components(graph.size(), -1);
    component component = 0;
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        auto v = *it;
        if (components[v] == -1)
            dfs2(v, component++, reversed_graph, components);
    }

    return components;
}

int main() {
    size_t n;

    std::cin >> n;

    std::vector<color> color(n);

    std::vector<std::pair<pin, pin>> w2p(n, {-1, -1});
    std::vector<wire> p2w(2 * n);

    //какой провод какого цвета
    for (size_t i = 0; i < n; ++i) {
        std::cin >> color[i] ;
    }

    //номер провода, который можно подсоединить к разьему i
    wire wire;
    for (int32_t i = 0; i < 2 * n; ++i) {
        std::cin >> wire;
        wire--;
        p2w[i] = wire;
        w2p[wire].first == -1 ? w2p[wire].first = i : w2p[wire].second = i;
    }

    adj_list graph(2 * n);
    adj_list reversed_graph(2 * n);

    auto next_pin = [n](int32_t i){return i % (2 * n);};

    for (int32_t n_pin = 0; n_pin < 2 * n; ++n_pin) {
        auto next_n_pin = next_pin(n_pin + 1);
        auto cur_wire = p2w[n_pin]; //номер провода в текущем пине
        auto next_wire = p2w[next_n_pin]; //номер провода в следующем

        if (color[cur_wire] == color[next_wire]) {
            pin next_bro = bro(next_wire, next_n_pin, w2p);
            pin cur_bro = bro(cur_wire, n_pin, w2p);

            graph[n_pin].push_back(next_bro);
            graph[next_n_pin].push_back(cur_bro);
//            std::cout << n_pin << " " << next_bro << " " << next_n_pin << " " << cur_bro << "\n";
            reversed_graph[next_bro].push_back(n_pin);
            reversed_graph[cur_bro].push_back(next_n_pin);
        }
    }

    auto component = find_components(graph, reversed_graph);

    for (auto [p1, p2] : w2p) {
        if (component[p1] == component[p2]) {
            std::cout << "NO";
            return 0;
        }
    }
    std::cout << "YES\n";
    for (auto [p1, p2] : w2p) {
        std::cout << (component[p1] > component[p2] ? p1 + 1 : p2 + 1) << " ";
    }
    return 0;
}
