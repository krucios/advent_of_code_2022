#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <utility>
#include <vector>
#include <stack>

using namespace std;

using range = pair<size_t, size_t>;

bool is_fully_contains(const range& first, const range& second) {
    return first.first <= second.first && first.second >= second.second;
}

bool has_overlap(const range& first, const range& second) {
    return (first.first >= second.first && first.first <= second.second)
        || (first.second >= second.first && first.second <= second.second);
}

range range_from_string(const string& s) {
    range ret;
    size_t pos = s.find('-');

    if (pos != std::string::npos) {
        string first_s  = s.substr(0, pos);
        string second_s = s.substr(pos + 1);

        ret.first  = stoi(first_s);
        ret.second = stoi(second_s);
    }

    return ret;
}

int main() {
    ifstream fin("5.in");

    string line;
    uint64_t ans_1 = 0;
    uint64_t ans_2 = 0;

    vector<stack<char> > stacks;

    bool init_completed = false;

    while (getline(fin, line)) {
        if (line == "") {
            init_completed = true;
        } else {
            if (init_completed) {
                stringstream ss(line);
                string dummy;
                size_t amount;
                size_t from;
                size_t to;

                ss >> dummy >> amount >> dummy >> from >> dummy >> to;

                from--;
                to--;

                vector<char> moved_together;
                for (size_t i = 0; i < amount; i++) {
                    moved_together.push_back(stacks[from].top());
                    stacks[from].pop();
                }

                reverse(begin(moved_together), end(moved_together));

                for (auto c : moved_together) {
                    stacks[to].push(c);
                }
            } else {
                reverse(begin(line), end(line));
                stacks.push_back(stack<char>());
                for (const char& c : line) {
                    stacks.back().push(c);
                }
            }
        }
    }

    string ans;
    for (const auto& s : stacks) {
        ans += s.top();
    }

    cout << ans << endl;

    return 0;
}