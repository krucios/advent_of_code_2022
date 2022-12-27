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

using coord_t = array<size_t, 3>;

constexpr size_t MAX = 22;

constexpr array<int, 6> dx = { 1,  0, -1,  0,  0,  0};
constexpr array<int, 6> dy = { 0, -1,  0,  1,  0,  0};
constexpr array<int, 6> dz = { 0,  0,  0,  0,  1, -1};

array<size_t, MAX * MAX * MAX> sides;
array<array<array<bool, MAX>, MAX>, MAX> m;

coord_t coord_from_string(const string& s) {
    coord_t ret;
    char comma;
    stringstream ss(s);

    ss >> ret[0] >> comma >> ret[1] >> comma >> ret[2];

    ret[0]++;
    ret[1]++;
    ret[2]++;

    return ret;
}

size_t coord_hash(const coord_t& c) {
    return c[0] * MAX * MAX + c[1] * MAX + c[2];
}

int main() {
    ifstream fin("18.in");

    for (auto& e : sides) { e = 0; }

    for (auto& slice : m) {
        for (auto& row : slice) {
            for (auto& cell : row) {
                cell = false;
            }
        }
    }

    string line;
    vector<coord_t> cubes;

    while (getline(fin, line)) {
        coord_t c = coord_from_string(line);
        cubes.push_back(c);
        sides[coord_hash(c)] = 6;
        m[c[0]][c[1]][c[2]] = true;
    }

    coord_t max_c = {0, 0, 0};
    for (const auto& c : cubes) {
        max_c[0] = max(max_c[0], c[0]);
        max_c[1] = max(max_c[0], c[1]);
        max_c[2] = max(max_c[0], c[2]);
    }

    cout << max_c[0] << " " << max_c[1] << " " << max_c[2] << endl;

    coord_t start = {0, 0, 0}; m[0][0][0] = true;
    queue<coord_t> bfs;
    bfs.push(start);

    while (!bfs.empty()) {
        coord_t c = bfs.front(); bfs.pop();

        for (size_t i = 0; i < dx.size(); i++) {
            if (((c[0] == 0 && dx[i] < 0) || (c[0] == (MAX - 1) && dx[i] > 0))
            ||  ((c[1] == 0 && dy[i] < 0) || (c[1] == (MAX - 1) && dy[i] > 0))
            ||  ((c[2] == 0 && dz[i] < 0) || (c[2] == (MAX - 1) && dz[i] > 0))) {
                continue;
            }

            coord_t n = {c[0] + dx[i], c[1] + dy[i], c[2] + dz[i]};

            if (m[n[0]][n[1]][n[2]] == false) {
                m[n[0]][n[1]][n[2]] = true;
                bfs.push(n);
            }
        }
    }

    for (size_t i = 0; i < MAX; i++) {
        for (size_t j = 0; j < MAX; j++) {
            for (size_t k = 0; k < MAX; k++) {
                if (!m[i][j][k]) {
                    cout << i << "," << j << "," << k << endl;

                    coord_t internal = {i, j, k};
                    cubes.push_back(internal);
                }
            }
        }
    }

    for (const auto& c : cubes) {
        for (size_t i = 0; i < dx.size(); i++) {
            coord_t neighbour = {c[0] + dx[i], c[1] + dy[i], c[2] + dz[i]};
            size_t neighbour_idx = coord_hash(neighbour);

            if (sides[neighbour_idx]) {
                sides[neighbour_idx]--;
            }
        }
    }

    uint64_t ans = 0;
    for (const auto& e : sides) { ans += e; };

    cout << ans << endl;

    return 0;
}