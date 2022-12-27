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

using int_t = long long;

enum type_t {
    VAL,
    ADD,
    SUB,
    MUL,
    DIV,
    UNKNOWN
};

string type_to_s(type_t t) {
    if (t == VAL) {
        return "VAL";
    } else if (t == ADD) {
        return "+";
    } else if (t == SUB) {
        return "-";
    } else if (t == MUL) {
        return "*";
    } else if (t == DIV) {
        return "/";
    }
    return "Unknown";
}

struct monkey_t {
    string name;
    type_t type;
    int_t  val;
    string first;
    string second;
    bool is_unknown = false;
};

map<string, monkey_t> monkeys;

monkey_t parse_monkey(const string& description) {
    monkey_t m;
    string s;

    stringstream ss(description);

    ss >> m.name;
    m.name.pop_back();

    ss >> s;

    if (isdigit(s[0])) {
        m.val = stoi(s);
        m.type = VAL;
    } else {
        m.first = s;

        ss >> s;
        if (s == "+") {
            m.type = ADD;
        } else if (s == "-") {
            m.type = SUB;
        } else if (s == "*") {
            m.type = MUL;
        } else if (s == "/") {
            m.type = DIV;
        }

        ss >> m.second;
    }

    return m;
}

void print_monkey(const monkey_t& m) {
    cout << m.name << ": ";

    if (m.type == VAL) {
        cout << m.val;
    } else {
        cout << m.first << " " << type_to_s(m.type) << " " << m.second;
    }

     cout << endl;
}

void propagate_unknown() {
    bool updated = false;

    do {
        updated = false;
        for (const auto& [curr, m] : monkeys) {
            if (m.is_unknown) {
                for (auto& [k, v] : monkeys) {
                    if (v.type != VAL && !v.is_unknown && (v.first == curr || v.second == curr)) {
                        v.is_unknown = true;
                        updated = true;
                    }
                }
            }
        }
    } while (updated);
}

int_t ask_monkey(const string& name) {
    monkey_t& m = monkeys[name];

    if (m.type == VAL) {
        return m.val;
    } else {
        int_t first  = ask_monkey(m.first);
        int_t second = ask_monkey(m.second);

        switch (m.type) {
            case ADD: return first + second;
            case SUB: return first - second;
            case MUL: return first * second;
            case DIV: return first / second;

            default: return -1;
        }
    }
}

int_t solve(const string& name, int_t val) {
    string unknown;
    string other;

    if (name == "humn") {
        return val;
    }

    monkey_t& m = monkeys[name];
    monkey_t& first  = monkeys[m.first];
    monkey_t& second = monkeys[m.second];

    if (first.is_unknown && second.is_unknown) {
        cout << "Both parts unknown" << endl;
        exit(1);
    }

    unknown = (first.is_unknown) ? first.name : second.name;
    other   = (first.is_unknown) ? second.name : first.name;

    int_t other_val = ask_monkey(other);
    int_t unknown_val = 0;

    switch (m.type) {
    case ADD:
        unknown_val = val - other_val;
        break;

    case SUB:
        unknown_val = (first.is_unknown) ? val + other_val : other_val - val;
        break;

    case MUL:
        unknown_val = val / other_val;
        break;

    case DIV:
        unknown_val = (first.is_unknown) ? val * other_val : other_val / val;
        break;

    default:
        cout << "Can't solve " << m.name << " of type " << type_to_s(m.type) << endl;
        exit(2);
        break;
    }
    return solve(unknown, unknown_val);
}

int main() {
    ifstream fin("21.in");

    string line;

    while (getline(fin, line)) {
        monkey_t m = parse_monkey(line);
        monkeys[m.name] = m;
    }

    cout << ask_monkey("root") << endl;

    monkeys["humn"].is_unknown = true;
    propagate_unknown();

    monkey_t& root = monkeys["root"];
    monkey_t& first  = monkeys[root.first];
    monkey_t& second = monkeys[root.second];

    string unknown  = (first.is_unknown) ? first.name : second.name;
    int_t known_val = (first.is_unknown) ? ask_monkey(second.name) : ask_monkey(first.name);

    cout << solve(unknown, known_val) << endl;

    return 0;
}
