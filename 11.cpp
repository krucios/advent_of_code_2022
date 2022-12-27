#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <utility>
#include <vector>
#include <queue>
#include <stack>
#include <array>
#include <map>

using namespace std;

using item_t = uint64_t;

enum operation_t {
    add_op,
    mul_op,
    sqr_op
};

struct monkey_t {
    queue<item_t> items;
    operation_t op;
    uint64_t operand;
    uint64_t test_div;
    size_t idx_true;
    size_t idx_false;
    size_t monkey_business;
};

item_t apply_op(item_t old, operation_t op, uint64_t operand) {
    switch (op) {
        case add_op:
            return old + operand;
        case mul_op:
            return old * operand;
        case sqr_op:
            return old * old;
    }
    return 0;
}

void print_monkeys(const vector<monkey_t>& monkeys) {
    for (size_t i = 0; i < monkeys.size(); i++) {
        const auto& m = monkeys[i];

        cout << "Monkey " << endl;
        cout << "\tHas " << m.items.size() << " items" << endl;
        cout << "\tOperation: new = old "
             << (m.op == add_op ? '+' : '*') << " "
             << (m.op == sqr_op ? "old" : to_string(m.operand)) << endl;

        cout << "\tTest: divisible by " << m.test_div << endl;
        cout << "\t\tIf true: throw to monkey " << m.idx_true << endl;
        cout << "\t\tIf false: throw to monkey " << m.idx_false << endl;

        cout << endl;

    }
}

int main() {
    ifstream fin("11.in");

    string line;
    size_t monkey_idx = 0;
    vector<monkey_t> monkeys;

    monkeys.resize(8);

    while (getline(fin, line)) {
        stringstream ss(line);
        string command;

        if (line == "") {
            continue;
        }

        ss >> command;

        if (command == "Monkey") {
            ss >> monkey_idx;

            monkeys[monkey_idx].items = {};
            monkeys[monkey_idx].monkey_business = 0;
            monkeys[monkey_idx].idx_true = -1;
            monkeys[monkey_idx].idx_false = -1;
        } else if (command == "Starting"){
            ss.ignore(8);

            while (!ss.eof()) {
                item_t item;

                ss >> item;
                monkeys[monkey_idx].items.push(item);

                ss.ignore(2);
            }
        } else if (command == "Operation:") {
            ss.ignore(11);

            char op;
            string operand;

            ss >> op >> operand;

            if (op == '+') {
                monkeys[monkey_idx].op = add_op;
                monkeys[monkey_idx].operand = stoi(operand);
            } else {
                if (operand == "old") {
                    monkeys[monkey_idx].op = sqr_op;
                    monkeys[monkey_idx].operand = -1;
                } else {
                    monkeys[monkey_idx].op = mul_op;
                    monkeys[monkey_idx].operand = stoi(operand);
                }
            }
        } else if (command == "Test:") {
            ss.ignore (14);

            uint64_t test_div;
            ss >> test_div;

            monkeys[monkey_idx].test_div = test_div;
        } else if (command == "If") {
            ss >> command;
            ss.ignore(17);

            size_t new_monkey_idx;
            ss >> new_monkey_idx;

            if (command == "true:") {
                monkeys[monkey_idx].idx_true = new_monkey_idx;
            } else {
                monkeys[monkey_idx].idx_false = new_monkey_idx;
            }
        }
    }

    uint64_t common = 1;
    for (const auto& m : monkeys) {
        common *= m.test_div;
    }
    cout << "Common: " << common << endl;

    for (size_t round = 0; round < 10000; round++) {
        for (size_t i = 0; i < monkeys.size(); i++) {
            auto& m = monkeys[i];

            while (m.items.size()) {
                item_t item = m.items.front();
                m.items.pop();

                m.monkey_business++;

                item = apply_op(item, m.op, m.operand);
                item = item % common;

                if (item % m.test_div) {
                    monkeys[m.idx_false].items.push(item);
                } else {
                    monkeys[m.idx_true].items.push(item);
                }
            }
        }
    }

    vector<size_t> monkey_business;
    monkey_business.resize(monkeys.size());

    for (size_t i = 0; i < monkeys.size(); i++) {
        monkey_business[i] = monkeys[i].monkey_business;
        cout << "Monkey " << i << ": inspected items " << monkeys[i].monkey_business << " times" << endl;
    }

    sort(rbegin(monkey_business), rend(monkey_business));
    cout << monkey_business[0] << " * " << monkey_business[1] << " = " << monkey_business[0] * monkey_business[1] << endl;

    return 0;
}