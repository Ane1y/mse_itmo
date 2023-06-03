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

    void remove(tkey_t key) {
        root = remove(root, key);
    }

    void insert(tkey_t key) {
        auto [left, right] = split(root, key);
        root = merge(merge(left, std::make_shared<Node>(key)), right);
    }

    int get_k_th_max(tkey_t key) {
        return get_key(get_k_th_max(root, get_size(root) - key));
    }

private:
    NodePtr root = nullptr;

    struct Node {
        tkey_t key;
        size_t size = 1;
        priority_t priority = dist(rd);
        tkey_t sum = key;
        bool is_op = false;
        NodePtr right = nullptr;
        NodePtr left = nullptr;
    };

    NodePtr get_k_th_max(NodePtr node, tkey_t key) {
        if (get_size(node->left) == key) {
            return node;
        }
        if (get_size(node->left) < key) {
            return get_k_th_max(node->right, key - get_size(node->left) - 1);
        }
        else {
            return get_k_th_max(node->left, key);
        }
    }

    NodePtr merge(NodePtr l, NodePtr r) {
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

    std::pair<NodePtr, NodePtr> split(NodePtr node, tkey_t key, tkey_t add = 0) {
        if (!node) {
            return {nullptr, nullptr};
        }
        if (key < get_key(node)) {
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

    NodePtr remove(NodePtr node, tkey_t key) {
        auto [left, right] = split(node, key);
        auto [new_left, _] = split(left, key - 1);
        return merge(new_left, right);
    }

    size_t get_size(NodePtr node) {
        return node == nullptr ? 0 : node->size;
    }

    void update(NodePtr node) {
        node->size = get_size(node->left) + get_size(node->right) + 1;
    }

    tkey_t get_key(NodePtr node) {
        return node == nullptr ? -1 : node->key;
    }

};
int main() {
    size_t n;
    int16_t op_type;
    tkey_t key;
    std::cin >> n;
    CartesianTree tree;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> op_type >> key;
        if (op_type == 1) {
            tree.insert(key);
        }
        if (op_type == 0) {
            std::cout << tree.get_k_th_max(key) << "\n";
        }
        if (op_type == -1) {
            tree.remove(key);
        }
    }

}

