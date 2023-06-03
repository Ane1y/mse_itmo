#include <iostream>
#include <vector>
#include <string>
#include <random>

class Container {
private:
    struct Node;
public:
    int build(std::vector<int32_t> v) {
        while (n < v.size()) {
            n <<= 1;
        }
        history.push_back(build(0, n - 1, v));
        return history.size() - 1;
    }
    int get_t(int right, int i) {
        return get_t(history[right], i, 0, n - 1);
    }
    int update(int r, int i, int x) {
        history.push_back(update(history[r], i, x, 0, n - 1));
        return history.size() - 1;
    }
    Node* update(Node* node, int i, int x, int left, int right) {
        Node *cur = new Node();
        if (left == right) {
            cur->key = x;
        } else {
            if (i <= ((left + right) >> 1)) {
                cur->left = update(node->left, i, x, left, (left + right) >> 1);
                cur->right = node->right;
            } else {
                cur->left = node->left;
                cur->right = update(node->right, i, x, ((left + right) >> 1) + 1, right);
            }
        }
        return cur;
    }

private:
    class Node {
    public:
        Node *left;
        Node *right;
        int key;
    };
    int n = 1;
    std::vector<Node *> history;

    int get_t(Node* node, int i, int left, int right) {
        if (left == right) {
            return node->key;
        }
        if (i <= ((left + right) >> 1)) {
            return get_t(node->left, i, left, (left + right) >> 1);
        } else {
            return get_t(node->right, i, ((left + right) >> 1) + 1, right);
        }
    }

    Node* build(int left, int right, std::vector<int> &v) {
        Node *node = new Node();
        if (left == right) {
            if (left < v.size()) {
                node->key = v[left];
            } else {
                node->key = 0;
            }
        } else {
            node->left = build(left, (left + right) >> 1, v);
            node->right = build(((left + right) >> 1) + 1, right, v);
        }
        return node;
    }
};

class PersistentQueue {
private:
    size_t idx = 0;
    std::vector<int> L, R;
    Container container;

public:
    explicit PersistentQueue(size_t q) {
        L.assign(q, 0);
        R.assign(q, 0);
        container.build(std::vector<int>(q, 0));
        idx++;
    }

    void push(int32_t r, int32_t x) {
        container.update(r, R[r], x);
        L[idx] = L[r];
        R[idx] = R[r] + 1;
        idx++;
    }

    int pop(int32_t version) {
        container.update(version, L[version], 0);
        L[idx] = L[version] + 1;
        R[idx] = R[version];
        idx++;
        return container.get_t(version, L[version]);
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t n;
    std::cin >> n;

    size_t t;
    int32_t m;

    std::string op;
    PersistentQueue queue(n + 1);
    for (int i = 0; i < n; ++i) {
        std::cin >> op;
        if (op == "1") {
            std::cin >> t >> m;
            queue.push(t, m);
        } else if (op == "-1") {
            std::cin >> t;
            std::cout << queue.pop(t) << "\n";
        }
    }
    return 0;
}