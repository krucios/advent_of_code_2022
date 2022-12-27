#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <utility>
#include <vector>
#include <stack>
#include <array>
#include <map>

using namespace std;

enum dir_e {
    U = 0,
    R = 1,
    D = 2,
    L = 3
};

constexpr static size_t MAP_SIZE = 1000;

using map_t = array<array<bool, MAP_SIZE>, MAP_SIZE>;
using pos_t = array<size_t, 2>;

dir_e dir_from_char(const char c) {
    if (c == 'U') {
        return U;
    } else if (c == 'R') {
        return R;
    } else if (c == 'D') {
        return D;
    } else {
        return L;
    }
}

void apply_move(pos_t& p, dir_e d) {
    switch (d) {
        case U: p[0]--; break;
        case R: p[1]++; break;
        case D: p[0]++; break;
        case L: p[1]--; break;
    }
}

void move_tail(pos_t& tail, pos_t head) {
    int x_diff = tail[0] - head[0];
    int y_diff = tail[1] - head[1];

    if (x_diff > 1) {
        tail[0]--;
        if (y_diff == 1) {
            tail[1]--;
        } else if (y_diff == -1) {
            tail[1]++;
        }
    } else if (x_diff < -1) {
        tail[0]++;
        if (y_diff == 1) {
            tail[1]--;
        } else if (y_diff == -1) {
            tail[1]++;
        }
    }

    if (y_diff > 1) {
        tail[1]--;
        if (x_diff == 1) {
            tail[0]--;
        } else if (x_diff == -1) {
            tail[0]++;
        }
    } else if (y_diff < -1) {
        tail[1]++;
        if (x_diff == 1) {
            tail[0]--;
        } else if (x_diff == -1) {
            tail[0]++;
        }
    }
}

int main() {
    ifstream fin("9.in");

    string line;
    pos_t h = {MAP_SIZE / 2, MAP_SIZE / 2};
    array<pos_t, 9> t;
    for (size_t i = 0; i < 9; i++) {
        t[i] = {MAP_SIZE / 2, MAP_SIZE / 2};
    }

    map_t m;

    for (size_t i = 0; i < MAP_SIZE; i++) {
        for (size_t j = 0; j < MAP_SIZE; j++) {
            m[i][j] = false;
        }
    }
    m[t[8][0]][t[8][1]] = true;

    while (getline(fin, line)) {
        string dir;
        string amount;

        stringstream ss(line);

        ss >> dir >> amount;

        dir_e d = dir_from_char(dir[0]);
        size_t dist = stoi(amount);

        for (size_t i = 0; i < dist; i++) {
            apply_move(h, d);
            move_tail(t[0], h);
            move_tail(t[1], t[0]);
            move_tail(t[2], t[1]);
            move_tail(t[3], t[2]);
            move_tail(t[4], t[3]);
            move_tail(t[5], t[4]);
            move_tail(t[6], t[5]);
            move_tail(t[7], t[6]);
            move_tail(t[8], t[7]);
            m[t[8][0]][t[8][1]] = true;
        }
    }

    size_t ans = 0;
    for (size_t i = 0; i < MAP_SIZE; i++) {
        for (size_t j = 0; j < MAP_SIZE; j++) {
            if (m[i][j]) ans++;
        }
    }

    cout << ans << endl;

    return 0;
}