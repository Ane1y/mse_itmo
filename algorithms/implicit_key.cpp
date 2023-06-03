#include <iostream>
#include <random>
#include <memory>

using priority_t = double;
using tkey_t = int64_t;

std::random_device rd;
std::uniform_real_distribution<priority_t> dist{0, 1};

class CartesianTree {
private:
    struct Node;
public:
    using NodePtr = std::shared_ptr<Node>;

    void add(size_t i, tkey_t x) {
        auto [left, right] = split(root, i);
        root = merge(merge(left, std::make_shared<Node>(x)), right);
    }

    void remove(size_t i) {
        root = remove(root, i);
    }

    void print() {
        print(root);
    }

    size_t size() {
        return get_size(root);
    }
private:
    struct Node {
        tkey_t key;
        size_t size = 1;
        tkey_t min = key;
        priority_t priority = dist(rd);
        NodePtr right = nullptr;
        NodePtr left = nullptr;
    };
    NodePtr root;

    size_t get_size(const NodePtr& node) {
        return node ? node->size : 0;
    }

    tkey_t get_min(const NodePtr& node) {
        return node ? node->min : std::numeric_limits<tkey_t>::max();
    }

    NodePtr update(const NodePtr& node) {
        if (node) {
            node->size = get_size(node->left) + get_size(node->right) + 1;
            node->min = std::min(std::min(get_min(node->left), get_min(node->right)), node->key);
        }
        return node;
    }

    void print(const NodePtr& node) {
        if (!node) {
            return;
        }
        print(node->left);
        std::cout << node->key << " ";
        print(node->right);
    }

    NodePtr remove(const NodePtr& node, tkey_t key) {
        auto [left, right] = split(node, key);
        auto [new_left, _] = split(left, key - 1);
        return merge(new_left, right);
    }

    [[nodiscard]] NodePtr merge(const NodePtr& l, const NodePtr& r) {
        if (!l) {
            return r;
        }
        if (!r) {
            return l;
        }
        if (l->priority > r->priority) {
            l->right = merge(l->right, r);
            update(l);
            return l;
        } else {
            r->left = merge(l, r->left);
            update(r);
            return r;
        }
    }

    std::pair<NodePtr, NodePtr> split(const NodePtr& node, size_t i) {
        if (!node) {
            return {nullptr, nullptr};
        }
        if (i <= get_size(node->left)) {
            auto [left, right] = split(node->left, i);
            node->left = update(right);
            return {update(left), update(node)};
        } else {
            auto [left, right] = split(node->right, i - get_size(node->left) - 1);
            node->right = update(left);
            return {update(node), update(right)};
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    size_t n, m;
    std::cin >> n >> m;
    tkey_t key;
    CartesianTree tree;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> key;
        tree.add(i, key);
    }
    std::string op;
    size_t pos;
    for (size_t i = 0; i < m; ++i) {
        std::cin >> op;
        if (op == "add") {
            std::cin >> pos >> key;
            tree.add(pos, key);
        }
        if (op == "del") {
            std::cin >> pos;
            tree.remove(pos);
        }
    }

    std::cout << tree.size() << std::endl;
    tree.print();
}