#include <iostream>
#include <random>
#include <memory>

using priority_t = double;
using tkey_t = int64_t;
//
std::random_device rd;
std::uniform_real_distribution<priority_t> dist{0, 1};

class CartesianTree {
private:
    struct Node;
public:
    using NodePtr = std::shared_ptr<Node>;

    void add(tkey_t key) {
        if (!find(root, key)) {
            auto [left, right] = split(root, key);
            root = merge(merge(left, std::make_shared<Node>(key)), right);
        }
    }

    tkey_t sum(size_t l, size_t r) {
        auto [left1, right1] = split(root, r + 1);
        auto [left2, right2] = split(left1, l);
        int64_t sum = 0;
        if (right2) {
            sum = get_sum(right2);
        }
        root = merge(left2, right2);
        root = merge(root, right1);
        return sum;
        //        auto [left1, right1] = split(history, logn);
//        NodePtr left2;
//        std::tie(right1, left2) = split(right1, r - logn + 1);
//        tkey_t sum = get_sum(right1);
//        history = merge(merge(left1, right1), left2);
        return sum;
    }

private:
    NodePtr root = nullptr;

    struct Node {
        tkey_t key;
        size_t get_size = 1;
        tkey_t sum = key;
        priority_t priority = dist(rd);
        NodePtr right = nullptr;
        NodePtr left = nullptr;
    };

    NodePtr find(NodePtr node, tkey_t key) {
        if (!node) {
            return nullptr;
        }

        if (get_key(node) == key) {
            return node;
        }
        if (get_key(node) < key) {
            return find(node->right, key);
        }
        return find(node->left, key);
    }

    [[nodiscard]] NodePtr merge(NodePtr l, NodePtr r) {
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

    [[nodiscard]] std::pair<NodePtr, NodePtr> split(NodePtr node, tkey_t key) {
        if (!node) {
            return {nullptr, nullptr};
        }
        if (get_key(node) >= key) {
            auto [left, right] = split(node->left, key);
            node->left = right;
            update(node);
            return {left, node};
        } else {
            auto [left, right] = split(node->right, key);// - get_size(node->left) - 1);
            node->right = left;
            update(node);
            return {node, right};
        }
    }


    [[nodiscard]] size_t get_size(NodePtr node) const {
        return !node ? 0 : node->get_size;
    }

    void update(NodePtr node) const {
        node->get_size = get_size(node->left) + get_size(node->right) + 1;
        node->sum = get_key(node) + get_sum(node->left) + get_sum(node->right);

    }

    [[nodiscard]] tkey_t get_sum(NodePtr node) const {
        return !node ? 0 : node->sum;
    }

    [[nodiscard]] tkey_t get_key(NodePtr node) const {
        return !node ? -1 : node->key;
    }

};
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    size_t n;
    std::cin >> n;
    char op;
    tkey_t num;
    size_t l, r;
    CartesianTree root;
    tkey_t last_op_result = std::numeric_limits<tkey_t>::min();
    for (size_t i = 0; i < n; ++i) {
        std::cin >> op;
        if (op == '+') {
            std::cin >> num;
            if (last_op_result != std::numeric_limits<tkey_t>::min()) {
                num = (num + last_op_result) % (tkey_t)std::pow(10, 9);
            }
            root.add(num);
            last_op_result = std::numeric_limits<tkey_t>::min();
        }
        if (op == '?') {
            std::cin >> l >> r;
            last_op_result = root.sum(l, r);
            std::cout << last_op_result << "\n";
        }
    }
}

