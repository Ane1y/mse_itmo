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

    void add(tkey_t i, tkey_t x) {
        auto [left, right] = split(root, i);
        root = merge(merge(left, std::make_shared<Node>(x)), right);
    }

    tkey_t min(tkey_t l, tkey_t r) {
        auto [left, right] = split(root, l - 1);
        auto [left1, right1] = split(right, r - l + 1);
        tkey_t answer = get_min(left1);
        root = merge(left, merge(left1, right1));
        return answer;
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

    std::pair<NodePtr, NodePtr> split(const NodePtr& node, tkey_t key) {
        if (!node) {
            return {nullptr, nullptr};
        }
        if (key <= get_size(node->left)) {
            auto [left, right] = split(node->left, key);
            node->left = update(right);
            return {update(left), update(node)};
        } else {
            auto [left, right] = split(node->right, key - get_size(node->left) - 1);
            node->right = update(left);
            return {update(node), update(right)};
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    size_t n;
    std::cin >> n;
    char op;
    tkey_t num, x, l, r;
    CartesianTree tree;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> op;
        if (op == '+') {
            std::cin >> x >> num;
            tree.add(x, num);
        }
        if (op == '?') {
            std::cin >> l >> r;
            std::cout << tree.min(l, r) << "\n";
        }
    }
}