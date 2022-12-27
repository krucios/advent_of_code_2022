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

using matrix_t = map<size_t, vector<size_t>>;
using opened_t = bitset<26 * 26>;

size_t vertex_hash(const string& s) {
    return (s[0] - 'A') * 26 + s[1] - 'A';
}

matrix_t m;
map<size_t, size_t> flow;
size_t total_flow = 0;
size_t ans_max = 2291; // Obtain from previous runs

void rec(size_t step, size_t curr, size_t el_curr, size_t prev, size_t el_prev, opened_t opened, size_t curr_flow, size_t total) {
    if ((total + total_flow * (26 - step)) < ans_max) return;

    if (step == 26) {
        if (total > ans_max) {
            ans_max = total;
            cout << "New max: " << ans_max << endl;
        }
        return;
    }

    step++;
    total += curr_flow;

    for (const auto& c : m[curr]) {
        if (c == prev) continue;

        for (const auto& e_c : m[el_curr]) {
            if (e_c == el_prev) continue;

            rec(step, c, e_c, curr, el_curr, opened, curr_flow, total);
        }

        if (!opened[el_curr] && flow[el_curr]) {
            opened.set(el_curr);
            rec(step, c, el_curr, curr, el_curr, opened, curr_flow + flow[el_curr], total);
            opened.reset(el_curr);
        }
    }

    if (!opened[curr] && flow[curr]) {
        opened.set(curr);
        curr_flow += flow[curr];

        for (const auto& e_c : m[el_curr]) {
            if (e_c == el_curr) continue;

            rec(step, curr, e_c, curr, el_curr, opened, curr_flow, total);
        }

        if (!opened[el_curr] && flow[el_curr]) {
            opened.set(el_curr);
            rec(step, curr, el_curr, curr, el_curr, opened, curr_flow + flow[el_curr], total);
        }
    }
}

int main() {
    ifstream fin("16.in");

    string line;

    while (getline(fin, line)) {
        stringstream ss(line);

        ss.ignore(6);

        string vertex;
        ss >> vertex;
        size_t v = vertex_hash(vertex);

        ss >> vertex >> vertex >> vertex;

        size_t f;
        ss >> f;
        flow[v] = f;
        total_flow += f;

        ss.ignore(2);
        ss >> vertex >> vertex >> vertex >> vertex;

        while (!ss.eof()) {
            string child;
            ss >> child;

            size_t c = vertex_hash(child);
            m[v].push_back(c);;
        }
    }

    opened_t opened;
    rec(0, vertex_hash("AA"), vertex_hash("AA"), -1, -1, opened, 0, 0);

    return 0;
}