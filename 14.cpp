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

using namespace std;

constexpr static size_t N = 1000;

using map_t = array<array<bool, N>, N>;
using coord_t = array<size_t, 2>;

map_t m;
map_t v;

coord_t read_coord(stringstream& ss) {
    coord_t ret;

    char comma;
    string arrow;

    ss >> ret[1] >> comma >> ret[0];

    if (!ss.eof()) ss >> arrow;

    return ret;
}

int main() {
    ifstream fin("14.in");

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            m[i][j] = false;
            v[i][j] = false;
        }
    }

    string line;
    size_t lowest = 0;
    while (getline(fin, line)) {
        stringstream ss(line);

        coord_t start, finish;

        start = read_coord(ss);
        lowest = max(lowest, start[0]);

        while (!ss.eof()) {
            finish = read_coord(ss);
            lowest = max(lowest, finish[0]);

            bool is_vertical = start[1] == finish[1];

            if (is_vertical) {
                for (size_t i = min(start[0], finish[0]); i <= max(start[0], finish[0]); i++) {
                    m[i][start[1]] = true;
                }
            } else {
                for (size_t j = min(start[1], finish[1]); j <= max(start[1], finish[1]); j++) {
                    m[start[0]][j] = true;
                }
            }

            start = finish;
        }
    }

    for (size_t i = 0; i < N; i++) {
        m[lowest + 2][i] = true;
    }

    size_t ans = 0;
    coord_t sand = {0, 500};

    while (sand[0] < lowest) {
        if (!m[sand[0] + 1][sand[1]] && !v[sand[0] + 1][sand[1]]) {
            sand[0] += 1;
        } else if (!m[sand[0] + 1][sand[1] - 1] && !v[sand[0] + 1][sand[1] - 1]) {
            sand[0] += 1;
            sand[1] -= 1;
        } else if (!m[sand[0] + 1][sand[1] + 1] && !v[sand[0] + 1][sand[1] + 1]) {
            sand[0] += 1;
            sand[1] += 1;
        } else {
            ans++;
            v[sand[0]][sand[1]] = true;
            sand[0] = 0;
            sand[1] = 500;
        }
    }

    cout << ans << endl;

    ans = 0;
    sand = {0, 500};
    bool stop = false;

    while (!stop) {
        if (!m[sand[0] + 1][sand[1]] && !v[sand[0] + 1][sand[1]]) {
            sand[0] += 1;
        } else if (!m[sand[0] + 1][sand[1] - 1] && !v[sand[0] + 1][sand[1] - 1]) {
            sand[0] += 1;
            sand[1] -= 1;
        } else if (!m[sand[0] + 1][sand[1] + 1] && !v[sand[0] + 1][sand[1] + 1]) {
            sand[0] += 1;
            sand[1] += 1;
        } else {
            ans++;
            v[sand[0]][sand[1]] = true;
            if (sand[0] == 0 && sand[1] == 500) {
                stop = true;
            } else {
                sand[0] = 0;
                sand[1] = 500;
            }
        }
    }

    cout << ans << endl;

    return 0;
}