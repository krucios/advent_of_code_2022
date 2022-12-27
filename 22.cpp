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

using map_t = vector<vector<char>>;
using coord_t = array<size_t, 2>;
using orientation_t = array<int, 2>;

map_t m;

constexpr array<int, 4> sign = {'>', 'V', '<', 'A'};
constexpr array<int, 4> dx = {0, 1, 0, -1};
constexpr array<int, 4> dy = {1, 0, -1, 0};

void print_map() {
    cout << endl;
    for (const auto& row : m) {
        for (const auto& c : row) {
            cout << c;
        }
        cout << endl;
    }
    cout << endl;
}

size_t rotate(size_t dir, char c) {
    size_t ret;

    if (c == 'R') {
        ret = (dir + 1) % 4;
    } else {
        ret = !dir ? 3 : (dir - 1);
    }

    return ret;
}

size_t play(const string& instr) {
    coord_t pos = {0, 0};
    size_t dir = 0;

    for (; pos[1] < m[0].size(); pos[1]++) { if (m[0][pos[1]] == '.') break; }

    stringstream ss(instr);

    while (!ss.eof()) {
        char cmd;
        size_t amount;
        bool is_move = isdigit(ss.peek());

        if (is_move) {
            ss >> amount;
        } else {
            ss >> cmd;
        }

        cout << "\nCurrent command: " << (is_move ? "move " : (cmd == 'L' ? "rotate left" : "rotate right")) << (is_move ? to_string(amount) : "") << endl;

        if (is_move) {
            for (size_t iter = 0; iter < amount; iter++) {
                coord_t next = {pos[0], pos[1]};
                size_t new_dir;
                bool did_jump = false;

                if (dir == 0) {
                    if (pos[1] == 149) {
                        did_jump = true;
                        new_dir = 2;
                        next[0] = 100 + (49 - pos[0]);
                        next[1] = 99;
                    } else if (pos[1] == 99 && pos[0] < 100 && pos[0] >= 50) {
                        did_jump = true;
                        new_dir = 3;
                        next[0] = 49;
                        next[1] = 100 + (pos[0] - 50);
                    } else if (pos[1] == 99 && pos[0] >= 100) {
                        did_jump = true;
                        new_dir = 2;
                        next[0] = 49 - (pos[0] - 100);
                        next[1] = 149;
                    } else if (pos[1] == 49 && pos[0] >= 150) {
                        did_jump = true;
                        new_dir = 3;
                        next[0] = 149;
                        next[1] = 50 + (pos[0] - 150);
                    }
                } else if (dir == 1) {
                    if (pos[0] == 49 && pos[1] >= 100) {
                        did_jump = true;
                        new_dir = 2;
                        next[0] = 50 + (pos[1] - 100);
                        next[1] = 99;
                    } else if (pos[0] == 149 && pos[1] >= 50) {
                        did_jump = true;
                        new_dir = 2;
                        next[0] = 150 + (pos[1] - 50);
                        next[1] = 49;
                    } else if (pos[0] == 199) {
                        did_jump = true;
                        new_dir = 1;
                        next[0] = 0;
                        next[1] = pos[1] + 100;
                    }
                } else if (dir == 2) {
                    if (pos[1] == 50 && pos[0] < 50) {
                        did_jump = true;
                        new_dir = 0;
                        next[0] = 100 + (49 - pos[0]);
                        next[1] = 0;
                    } else if (pos[1] == 50 && pos[0] >= 50 && pos[0] < 100) {
                        did_jump = true;
                        new_dir = 1;
                        next[0] = 100;
                        next[1] = pos[0] - 50;
                    } else if (pos[1] == 0 && pos[0] < 150) {
                        did_jump = true;
                        new_dir = 0;
                        next[0] = 149 - pos[0];
                        next[1] = 50;
                    } else if (pos[1] == 0 && pos[0] >= 150) {
                        did_jump = true;
                        new_dir = 1;
                        next[0] = 0;
                        next[1] = 50 + (pos[0] - 150);
                    }
                } else {
                    if (pos[0] == 0 && pos[1] >= 50 && pos[1] < 100) {
                        did_jump = true;
                        new_dir = 0;
                        next[0] = 150 + (pos[1] - 50);
                        next[1] = 0;
                    } else if (pos[0] == 0 && pos[1] >= 100) {
                        did_jump = true;
                        new_dir = 3;
                        next[0] = 199;
                        next[1] = pos[1] - 100;
                    } else if (pos[0] == 100 && pos[1] < 50) {
                        did_jump = true;
                        new_dir = 0;
                        next[0] = 50 + pos[1];
                        next[1] = 50;
                    }
                }

                if (!did_jump) {
                    next[0] = next[0] + dx[dir];
                    next[1] = next[1] + dy[dir];
                }

                if (m[next[0]][next[1]] == '#') {
                    break;
                } else { // if (m[next[0]][move[1]] == '.') {
                    m[pos[0]][pos[1]] = sign[dir];
                    pos = next;
                    if (did_jump) {
                        dir = new_dir;
                    }
                }
            }
        } else {
            dir = rotate(dir, cmd);
        }

        // print_map();
    }

    return (pos[0] + 1) * 1000 + (pos[1] + 1) * 4 + dir;
}

int main() {
    ifstream fin("22.in");

    for (auto& row : m) {
        for (auto& c : row) {
            c = ' ';
        }
    }

    string line;
    string instr;
    bool is_reading_map = true;
    size_t row = 0;
    size_t ans;

    while (getline(fin, line)) {
        if (line == "") {
            is_reading_map = false;
        } else {
            if (is_reading_map) {
                m.push_back(vector<char>());
                for (size_t col = 0; col < line.length(); col++) {
                    m.back().push_back(line[col]);
                }
                row++;
            } else {
                ans = play(line);
            }
        }
    }

    print_map();

    cout << ans << endl;

    return 0;
}

// 40290 - too high