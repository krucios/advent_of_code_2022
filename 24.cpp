#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <utility>
#include <numeric>
#include <vector>
#include <queue>
#include <stack>
#include <array>
#include <map>
#include <set>

using namespace std;

constexpr array<int, 4> dx = {-1, 1, 0, 0};
constexpr array<int, 4> dy = {0, 0, -1, 1};

struct blizzard_t {
    int x;
    int y;
    int d;
};

char dir_to_char(int dir) {
    return dir == 0 ? '^'
         : dir == 1 ? 'V'
         : dir == 2 ? '<'
         : dir == 3 ? '>'
         :            'X';
}

int char_to_dir(char c) {
    return c == '^' ? 0
         : c == 'v' ? 1
         : c == '<' ? 2
         : c == '>' ? 3
         : -1;
}

using blizzards_t = vector<blizzard_t>;
using coord_t = array<int, 3>;
using map_t = vector<vector<char>>;

int rows;
int cols;
int period;

map_t m;
vector<map_t> maps;
blizzards_t blizzards;

map_t apply_blizzards() {
    map_t ret = m;

    for (const auto& b : blizzards) {
        char c = dir_to_char(b.d);

        ret[b.x][b.y] = c;
    }

    return ret;
}

void advance_blizzards() {
    for (auto& b : blizzards) {
        switch (b.d) {
        case 0:
            b.x = (b.x == 1) ? m.size() - 2 : b.x - 1;
            break;

        case 1:
            b.x = (b.x == (m.size() - 2)) ? 1 : b.x + 1;
            break;

        case 2:
            b.y = (b.y == 1) ? m[0].size() - 2 : b.y - 1;
            break;

        case 3:
            b.y = (b.y == (m[0].size() - 2)) ? 1 : b.y + 1;
            break;

        default:
            cout << "Unexpected direction " << b.d << endl;
            exit(1);
            break;
        }
    }
}

void print(map_t m) {
    for (size_t i = 0; i < m.size(); i++) {
        for (size_t j = 0; j < m[0].size(); j++) {
            cout << m[i][j];
        }
        cout << endl;
    }
}

int bfs(coord_t start, coord_t finish) {
    vector<vector<vector<int>>> min_dist;

    for (size_t i = 0; i < period; i++) {
        min_dist.push_back(vector<vector<int>>());
        for (size_t j = 0; j < rows; j++) {
            min_dist[i].push_back(vector<int>());
            for (size_t k = 0; k < cols; k++) {
                min_dist[i][j].push_back(UINT16_MAX);
            }
        }
    }

    queue<coord_t> bfs;
    min_dist[start[0]][start[1]][start[2]] = 0;
    bfs.push(start);

    while (!bfs.empty()) {
        coord_t c = bfs.front(); bfs.pop();
        int curr_dist = min_dist[c[0]][c[1]][c[2]];
        int new_t = (c[0] + 1) % period;

        if (maps[new_t][c[1]][c[2]] == '.' && min_dist[new_t][c[1]][c[2]] > (curr_dist + 1)) {
            min_dist[new_t][c[1]][c[2]] = curr_dist + 1;
            bfs.push({new_t, c[1], c[2]});
        }

        if (c[1] > 0 && maps[new_t][c[1] - 1][c[2]] == '.' && min_dist[new_t][c[1] - 1][c[2]] > (curr_dist + 1)) {
            min_dist[new_t][c[1] - 1][c[2]] = curr_dist + 1;
            bfs.push({new_t, c[1] - 1, c[2]});
        }

        if (c[1] < (rows - 1) && maps[new_t][c[1] + 1][c[2]] == '.' && min_dist[new_t][c[1] + 1][c[2]] > (curr_dist + 1)) {
            min_dist[new_t][c[1] + 1][c[2]] = curr_dist + 1;
            bfs.push({new_t, c[1] + 1, c[2]});
        }

        if (c[2] > 0 && maps[new_t][c[1]][c[2] - 1] == '.' && min_dist[new_t][c[1]][c[2] - 1] > (curr_dist + 1)) {
            min_dist[new_t][c[1]][c[2] - 1] = curr_dist + 1;
            bfs.push({new_t, c[1], c[2] - 1});
        }

        if (c[2] < (cols - 1) && maps[new_t][c[1]][c[2] + 1] == '.' && min_dist[new_t][c[1]][c[2] + 1] > (curr_dist + 1)) {
            min_dist[new_t][c[1]][c[2] + 1] = curr_dist + 1;
            bfs.push({new_t, c[1], c[2] + 1});
        }
    }

    int ans = UINT16_MAX;
    for (size_t i = 0; i < period; i++) {
        ans = min(ans, min_dist[i][finish[1]][finish[2]]);
    }

    return ans;
}

int main() {
    ifstream fin("24.in");

    m.clear();
    blizzards.clear();

    string line;
    int i = 0;
    while (getline(fin, line)) {
        m.push_back(vector<char>());

        for (int j = 0; j < line.length(); j++) {
            char c = line[j];

            if (c == '#' || c == '.') {
                m.back().push_back(c);
            } else {
                blizzard_t b;
                b.x = i;
                b.y = j;
                b.d = char_to_dir(c);
                blizzards.push_back(b);
                m.back().push_back('.');
            }
        }

        i++;
    }

    rows = m.size();
    cols = m[0].size();
    period = lcm(rows, cols);

    maps.push_back(apply_blizzards());
    for (size_t i = 0; i < period - 1; i++) {
        advance_blizzards();
        maps.push_back(apply_blizzards());
    }

    int part_2 = 0;
    int min_dist;
    min_dist = bfs({0, 0, 1}, {0, rows - 1, cols - 2});
    part_2 += min_dist;
    cout << min_dist << endl;

    min_dist = bfs({part_2 % period, rows - 1, cols - 2}, {0, 0, 1});
    part_2 += min_dist;
    cout << min_dist << endl;

    min_dist = bfs({part_2 % period, 0, 1}, {0, rows - 1, cols - 2});
    part_2 += min_dist;
    cout << min_dist << endl;
    cout << part_2 << endl;

    return 0;
}
