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

constexpr size_t MAX = 1000;
constexpr size_t shift = 500;

using map_t = array<array<bool, MAX>, MAX>;
using coord_t = array<size_t, 2>;

enum dir_t {
    N = 0,
    S = 1,
    W = 2,
    E = 3,
    STAY = -1
};

constexpr array<int, 4> dx = {-1, 1, 0, 0};
constexpr array<int, 4> dy = {0, 0, -1, 1};

dir_t dir = N;
vector<coord_t> elves;
map_t m;

size_t coord_hash(coord_t c) {
    return c[0] * MAX + c[1];
}

dir_t where_to_go(coord_t e) {
    bool has_adjacent_n = m[e[0] - 1][e[1] - 1]
                        | m[e[0] - 1][e[1]    ]
                        | m[e[0] - 1][e[1] + 1];
    bool has_adjacent_e = m[e[0] - 1][e[1] + 1]
                        | m[e[0]    ][e[1] + 1]
                        | m[e[0] + 1][e[1] + 1];
    bool has_adjacent_s = m[e[0] + 1][e[1] + 1]
                        | m[e[0] + 1][e[1]    ]
                        | m[e[0] + 1][e[1] - 1];
    bool has_adjacent_w = m[e[0] - 1][e[1] - 1]
                        | m[e[0] + 1][e[1] - 1]
                        | m[e[0]    ][e[1] - 1];

    if (!(has_adjacent_n | has_adjacent_e | has_adjacent_s | has_adjacent_w)) {
        return STAY;
    }

    for (int i = 0; i < 4; i++) {
        dir_t curr_dir = dir_t((dir + i) % 4);

        switch (curr_dir) {
        case N:
            if (!has_adjacent_n) return N;
            break;

        case E:
            if (!has_adjacent_e) return E;
            break;

        case S:
            if (!has_adjacent_s) return S;
            break;

        case W:
            if (!has_adjacent_w) return W;
            break;

        default:
            cout << "Wrong direction" << endl;
            exit(1);
            break;
        }
    }

    return STAY;
}

coord_t propose(coord_t e) {
    dir_t d = where_to_go(e);

    coord_t ret;
    if (d == STAY) {
        ret = e;
    } else {
        ret = {e[0] + dx[d], e[1] + dy[d]};
    }

    return ret;
}

map_t new_m;
map<size_t, size_t> proposed;

bool round() {
    bool moved = false;

    new_m = m;
    proposed.clear();

    for (const auto& e : elves) {
        coord_t p = propose(e);
        size_t hash = coord_hash(p);
        proposed[hash] = proposed[hash] + 1;
    }

    for (auto& e : elves) {
        coord_t p = propose(e);
        size_t hash = coord_hash(p);

        // Maybe clear first and then assign new positions?
        if (e != p && proposed[hash] == 1) {
            moved = true;
            new_m[e[0]][e[1]] = false;
            new_m[p[0]][p[1]] = true;
            e = p;
        }
    }

    m = new_m;
    dir = dir_t((dir + 1) % 4);

    return moved;
}

pair<coord_t, coord_t> rectangle() {
    coord_t left_top = elves[0];
    coord_t right_bot = elves[0];

    for (const auto& e : elves) {
        left_top[0]  = min(left_top[0],  e[0]);
        left_top[1]  = min(left_top[1],  e[1]);
        right_bot[0] = max(right_bot[0], e[0]);
        right_bot[1] = max(right_bot[1], e[1]);
    }

    return make_pair(left_top, right_bot);
}

void print() {
    pair<coord_t, coord_t> r = rectangle();

    for (size_t i = r.first[0]; i <= r.second[0]; i++) {
        for (size_t j = r.first[1]; j <= r.second[1]; j++) {
            cout << (m[i][j] ? '#' : '.');
        }
        cout << endl;
    }
}

size_t ans() {
    size_t ret = 0;
    pair<coord_t, coord_t> r = rectangle();

    return (r.second[0] - r.first[0] + 1) * (r.second[1] - r.first[1] + 1) - elves.size();
}

int main() {
    ifstream fin("23.in");

    for (auto& r : m) { for (auto& c : r) { c = false; }}
    elves.clear();

    string line;
    int idx = 0;

    while (getline(fin, line)) {
        for (int j = 0; j < line.length(); j++) {
            if (line[j] == '#') {
                coord_t elf = {idx + shift, j + shift};
                m[elf[0]][elf[1]] = true;
                elves.push_back(elf);
            }
        }
        idx++;
    }

    print();

    size_t rounds = 1;
    while (round()) {
        rounds++;
        cout << rounds << endl;
    }

    cout << rounds << endl;

    return 0;
}
