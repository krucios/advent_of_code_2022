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

using map_t = vector<vector<int>>;

struct coord_t {
    int x;
    int y;
};

enum dir_t {
    U = 0,
    R = 1,
    D = 2,
    L = 3
};

constexpr static array<int, 4> dx = {-1, 0, 1, 0};
constexpr static array<int, 4> dy = {0, 1, 0, -1};

int main() {
    ifstream fin("12.in");

    string line;
    map_t hills;
    map_t dist;
    size_t n = 0;
    size_t m = 0;
    coord_t s;
    coord_t e;

    while (getline(fin, line)) {
        hills.push_back(vector<int>());
        m = 0;

        for (char c : line) {
            size_t height;

            if (c == 'S') {
                height = 1;
                s.x = n;
                s.y = m;
            } else if (c == 'E') {
                height = 'z' - 'a';
                e.x = n;
                e.y = m;
            } else {
                height = c - 'a' + 1;
            }

            hills.back().push_back(height);
            m++;
        }
        n++;
    }

    cout << "Dimensions: " << n << " x " << m << endl;

    dist.resize(n);
    for (auto& row : dist) {
        row.resize(m);
        for (auto& e : row) {
            e = 1000;
        }
    }

    dist[e.x][e.y] = 0;
    queue<coord_t> bfs;
    bfs.push(e);

    while (!bfs.empty()) {
        coord_t curr = bfs.front();
        bfs.pop();

        size_t new_dist = dist[curr.x][curr.y] + 1;

        for (int i = 0; i < 4; i++) {
            coord_t to_visit = curr;
            to_visit.x += dx[i];
            to_visit.y += dy[i];

            if (to_visit.x < 0 || to_visit.x == n
            ||  to_visit.y < 0 || to_visit.y == m) {
                continue;
            }

            if ((hills[to_visit.x][to_visit.y] >= (hills[curr.x][curr.y] - 1))
            &&  (dist[to_visit.x][to_visit.y] > new_dist)) {
                dist[to_visit.x][to_visit.y] = new_dist;
                bfs.push(to_visit);
            }
        }
    }

    cout << dist[e.x][e.y] << endl;
    cout << dist[s.x][s.y] << endl;

    int ans = 100000;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (hills[i][j] == 1 && dist[i][j] < ans) {
                ans = dist[i][j];
            }
        }
    }

    cout << ans << endl;

    return 0;
}