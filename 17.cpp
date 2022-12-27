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

constexpr static size_t W = 7;
constexpr static size_t H = 1000000;

constexpr static size_t S_W = 7;
constexpr static size_t S_H = 4;

using shape_t = array<array<bool, S_W>, S_H>;
using map_t = array<array<bool, W>, H>;

struct block_t {
    size_t h;
    shape_t s;
};

constexpr shape_t line_h = {{
    {0, 0, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0}
}};

constexpr shape_t cross = {{
    {0, 0, 0, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0}
}};

constexpr shape_t corner = {{
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0}
}};

constexpr shape_t line_v = {{
    {0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0}
}};

constexpr shape_t square = {{
    {0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0}
}};

constexpr array<shape_t, 5> shapes = {line_h, cross, corner, line_v, square};

map_t m;

void move_sideways(block_t& b, char d) {
    bool is_right = d == '>';
    bool can_move = true;

    // The movement won't move the block into walls
    if (is_right) {
        for (size_t i = 0; i < S_H; i++) {
            can_move &= !b.s[i][S_W - 1];
        }
    } else {
        for (size_t i = 0; i < S_H; i++) {
            can_move &= !b.s[i][0];
        }
    }

    // The movement won't move the block into the existing structure
    int dw = is_right ? 1 : -1;
    for (size_t i = 0; i < S_H; i++) {
        for (size_t j = is_right ? 0 : 1; j < S_W - (is_right ? 1 : 0); j++) {
            can_move &= (!b.s[i][j] || !m[b.h + i][j + dw]);
        }
    }

    if (can_move) {
        shape_t new_shape;

        for (size_t i = 0; i < S_H; i++) {
            for (size_t j = is_right ? 0 : 1; j < S_W - (is_right ? 1 : 0); j++) {
                new_shape[i][j + dw] = b.s[i][j];
            }
        }

        for (size_t i = 0; i < S_H; i++) {
            new_shape[i][is_right ? 0 : (S_W - 1)] = false;
        }

        b.s = new_shape;
    }
}

bool move_downwards(block_t& b) {
    bool can_move = b.h > 0;

    if (can_move) {
        for (size_t i = 0; i < S_H; i++) {
            for (size_t j = 0; j < S_W; j++) {
                can_move &= (!b.s[i][j] || !m[b.h + i - 1][j]);
            }
        }
    }

    if (can_move) {
        b.h--;
    }

    return can_move;
}

void place_block(const block_t& b) {
    for (size_t i = 0; i < S_H; i++) {
        for (size_t j = 0; j < S_W; j++) {
            m[b.h + i][j] |= b.s[i][j];
        }
    }
}

size_t max_h(size_t curr_h) {
    for (size_t i = curr_h; i < H; i++) {
        bool has_rock = false;

        for (size_t j = 0; j < W; j++) {
            has_rock |= m[i][j];
        }

        if (!has_rock) {
            return i;
        }
    }
    return H;
}

bool as_floor(size_t h) {
    for (size_t i = 0; i < W; i++) {
        if (!m[h][i]) return false;
    }
    return true;
}

void print_map(size_t max_h) {
    for (int i = max_h; i >= 0; i--) {
        cout << "|";
        for (size_t j = 0; j < W; j++) {
            cout << (m[i][j] ? '#' : '.');
        }
        cout << "|" << endl;
    }
    cout << "+-------+" << endl;
}

int main() {
    ifstream fin("17.in");

    for (auto& row : m) {
        for (auto& e : row) e = false;
    }

    string line;
    getline(fin, line);
    cout << "Gas pattern length: " << line.length() << endl;

    uint64_t i_init = 1168;
    uint64_t i_diff = 1735;

    uint64_t h_init = 1848;
    uint64_t h_diff = 2781;

    size_t   shape_start = 4;
    size_t   jet_start   = 6812;

    uint64_t repetitions = (1000000000000ULL - i_init - 1) / i_diff;
    uint64_t steps_left  = (1000000000000ULL - i_init - 1) % i_diff;

    uint64_t total_h = h_init + h_diff * repetitions;

    for (auto& row : m) {
        for (auto& e : row) e = false;
    }

    m[1] = {0, 0, 1, 0, 0, 0, 1};
    m[0] = {0, 0, 1, 0, 1, 0, 1};

    size_t shape_idx = 0;
    size_t jet_idx = 0;
    size_t curr_h = 0;
    size_t last_h = 0;

    for (size_t i = 0; i < steps_left; i++) {
        // New rock appear
        block_t b;
        b.h = curr_h + 3;
        b.s = shapes[shape_idx]; shape_idx = (shape_idx + 1) % shapes.size();

        bool is_fixed = false;

        // ... and fall until it's fixed
        while (!is_fixed) {
            // firstly, jet effect applies
            move_sideways(b, line[jet_idx]); jet_idx = (jet_idx + 1) % line.length();

            // ... then downward movement
            is_fixed = !move_downwards(b);
        }

        place_block(b);
        curr_h = max_h(curr_h);
    }

    total_h += curr_h - 1;

    cout << "Max height: " << total_h << endl;

    return 0;
}