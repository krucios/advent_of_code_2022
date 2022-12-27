#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <utility>
#include <vector>
#include <queue>
#include <stack>
#include <array>
#include <map>
#include <set>

using namespace std;

using val_t = array<int64_t, 2>;

struct node_t {
    val_t val;
    node_t* prev = nullptr;
    node_t* next = nullptr;
};

class DLL {
    node_t* head;
    size_t dll_size;

    public:
    DLL() : head(nullptr), dll_size(0) {}

    ~DLL() {
        node_t* next = head;

        for (size_t i = 0; i < dll_size; i++) {
            head = next;
            next = head->next;
            delete head;
        }
    }

    void push_back(val_t val) {
        node_t* n = new node_t;
        n->val = val;
        dll_size++;

        if (head != nullptr) {
            n->prev = head->prev;
            n->next = head;

            head->prev->next = n;
            head->prev = n;
        } else {
            head = n;
            n->next = n;
            n->prev = n;
        }
    }

    val_t& at(int pos) const {
        node_t* n = head;
        bool is_forward = pos >= 0;

        if (is_forward) {
            while (pos--) {
                n = n->next;
            }
        } else {
            while (pos++) {
                n = n->prev;
            }
        }

        return n->val;
    }

    void move(val_t v) {
        node_t* prev;
        node_t* next;
        node_t* next_next;
        node_t* prev_prev;
        node_t* curr = head;

        for (size_t i = 0; i < dll_size; i++) {
            if (curr->val[0] == v[0] && curr->val[1] == v[1]) {
                break;
            }
            curr = curr->next;
        }

        if (v[0] == 0) return;

        bool is_forward = v[0] >= 0;
        size_t k = abs(v[0]) % dll_size;

        if (k > 0 && head == curr) {
            head = curr->next;
        }

        if (is_forward) {
            while (k--) {
                prev = curr->prev;
                next = curr->next;
                next_next = next->next;

                prev->next = next;
                next->prev = prev;
                next->next = curr;
                curr->prev = next;
                curr->next = next_next;
                next_next->prev = curr;
            }
        } else {
            while (k--) {
                prev = curr->prev;
                next = curr->next;
                prev_prev = prev->prev;

                next->prev = prev;
                prev->next = next;
                prev->prev = curr;
                curr->next = prev;
                curr->prev = prev_prev;
                prev_prev->next = curr;
            }
        }
    }

    void print() const {
        node_t* curr = head;

        for (size_t i = 0; i < dll_size; i++) {
            cout << curr->val[0] << " ";
            curr = curr->next;
        }
        cout << endl;
    }

    int64_t ans() const {
        node_t* curr = head;

        for (size_t i = 0; i < dll_size; i++) {
            if (curr->val[0] == 0) {
                break;
            }
            curr = curr->next;
        }

        int64_t ret = 0;
        for (size_t i = 0; i < 3; i++) {
            for (size_t j = 0; j < 1000; j++) {
                curr = curr->next;
            }
            ret += curr->val[0];
        }

        return ret;
    }


    val_t& operator[](int pos) const { return at(pos); }
    size_t size() const { return dll_size; }

    DLL(const DLL& other) = delete;
    DLL& operator=(const DLL& other) = delete;
};

int main() {
    ifstream fin("20.in");

    string line;
    size_t idx;

    DLL dll;
    vector<val_t> original;

    while (getline(fin, line)) {
        val_t v;
        v[0] = stoi(line) * 811589153LL;
        v[1] = idx++;

        dll.push_back(v);
        original.push_back(v);
    }

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < original.size(); j++) {
            dll.move(original[j]);
            // dll.print();
        }
    }

    cout << dll.ans() << endl;

    return 0;
}

// 6948826327986 - too low
// 14621590180448 - too high