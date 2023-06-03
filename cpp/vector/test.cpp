#include "vector.hpp"

#include <cassert>

struct IntHolder {
    static inline bool do_throw = false;
    int v;

    explicit IntHolder(int i): v{i} {}

    IntHolder(const IntHolder& i): v{i.v} {
        if (do_throw) {
            throw std::exception();
        }
    }
    IntHolder(IntHolder&& i): v{i.v} {
        if (do_throw) {
            throw std::exception();
        }
    }
    IntHolder& operator=(const IntHolder& i) {
        v = i.v;
        if (do_throw) {
            throw std::exception();
        }
        return *this;
    }
    IntHolder& operator=(IntHolder&& i) {
        v = i.v;
        if (do_throw) {
            throw std::exception();
        }
        return *this;
    }
};

void test_ints_vector() {
    cls01::Vector<int> v(100, 42);
    assert(v.size() == 100);

    v.push_back(24);
    v.push_back(24);

    assert(v.size() == 102);
}

void test_int_holders_vector() {
    cls01::Vector<IntHolder> v(100, IntHolder{42});
    assert(v.size() == 100);

    IntHolder::do_throw = true;
    bool exception_caught = false;

    try {
        IntHolder ih{24};
        v.push_back(ih);
    } catch (const std::exception& e) {
        exception_caught = true;
    }

    assert(exception_caught);
    assert(v.size() == 100);
}

int main() {
    test_ints_vector();
    test_int_holders_vector();
}
