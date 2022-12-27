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

enum direction {
    top = 0,
    right = 1,
    bottom = 2,
    left = 3
};

using forest_t = vector<vector<size_t>>;
using visibility_t = array<size_t, 4>;
using vis_map_t = vector<vector<visibility_t>>;

void print_forest(const forest_t& forest) {
    for (const auto& row : forest) {
        for (const auto& e : row) {
            cout << e;
        }
        cout << endl;
    }
}

uint64_t scenic_score(size_t x, size_t y, const forest_t& forest) {
    uint64_t top_score = 0;
    uint64_t right_score = 0;
    uint64_t bottom_score = 0;
    uint64_t left_score = 0;

    size_t h = forest[x][y];

    for (int i = x - 1; i >= 0; i--) {
        if (h > forest[i][y]) {
            top_score++;
        } else {
            top_score++;
            break;
        }
    }

    for (int i = x + 1; i < forest.size(); i++) {
        if (h > forest[i][y]) {
            bottom_score++;
        } else {
            bottom_score++;
            break;
        }
    }

    for (int j = y - 1; j >= 0; j--) {
        if (h > forest[x][j]) {
            left_score++;
        } else {
            left_score++;
            break;
        }
    }

    for (int j = y + 1; j < forest[0].size(); j++) {
        if (h > forest[x][j]) {
            right_score++;
        } else {
            right_score++;
            break;
        }
    }

    return top_score * right_score * bottom_score * left_score;
}

int main() {
    ifstream fin("8.in");

    string line;
    forest_t forest;
    vis_map_t vis_map;

    while (getline(fin, line)) {
        forest.push_back(vector<size_t>());
        vis_map.push_back(vector<visibility_t>());

        for (const auto& e : line) {
            forest.back().push_back(static_cast<size_t>(e - '0'));
            vis_map.back().push_back({});
        }
    }

    size_t n = forest.size();
    size_t m = forest[0].size();

    for (size_t i = 1; i < n - 1; i++) {
        for (size_t j = 1; j < m - 1; j++) {
            vis_map[i][j][direction::top   ] = max(vis_map[i - 1][j    ][direction::top   ], forest[i - 1][j    ]);
            vis_map[i][j][direction::left  ] = max(vis_map[i    ][j - 1][direction::left  ], forest[i    ][j - 1]);
        }
    }

    for (size_t i = n - 2; i > 0; i--) {
        for (size_t j = m - 2; j > 0; j--) {
            vis_map[i][j][direction::right ] = max(vis_map[i    ][j + 1][direction::right ], forest[i    ][j + 1]);
            vis_map[i][j][direction::bottom] = max(vis_map[i + 1][j    ][direction::bottom], forest[i + 1][j    ]);
        }
    }

    size_t ans = n * 2 + m * 2 - 4;
    for (size_t i = 1; i < n - 1; i++) {
        for (size_t j = 1; j < m - 1; j++) {
            if ((forest[i][j] > vis_map[i][j][direction::top])
            ||  (forest[i][j] > vis_map[i][j][direction::right])
            ||  (forest[i][j] > vis_map[i][j][direction::bottom])
            ||  (forest[i][j] > vis_map[i][j][direction::left])) {
                ans++;
            }
        }
    }

    cout << "First part: " << ans << endl;

    cout << "Dimensions: " << n << " x " << m << endl;

    uint64_t best_score = 0;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            uint64_t score = scenic_score(i, j, forest);

            best_score = max(score, best_score);
        }
    }

    cout << "Second part: " << best_score << endl;


    return 0;
}