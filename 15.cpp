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

using coord_t  = array<int, 2>;
using sensor_t = pair<coord_t, coord_t>;

size_t dist(const coord_t& a, const coord_t& b) {
    return abs(a[0] - b[0]) + abs(a[1] - b[1]);
}

int main() {
    ifstream fin("15.in");

    vector<sensor_t> sensors;
    string line;

    while (getline(fin, line)) {
        coord_t sensor;
        coord_t beacon;

        stringstream ss(line);

        ss.ignore(12);
        ss >> sensor[0];

        ss.ignore(4);
        ss >> sensor[1];

        ss.ignore(25);
        ss >> beacon[0];

        ss.ignore(4);
        ss >> beacon[1];

        sensors.push_back(make_pair(sensor, beacon));
    }

    int target_y = 2000000;
    set<int> ans;
    for (const auto& [s, b] : sensors) {
        size_t d = dist(s, b);
        size_t diff = abs(s[1] - target_y);

        if (diff <= d) {
            int lower = s[0] - (d - diff);
            int upper = s[0] + (d - diff);

            for (int i = lower; i <= upper; i++) {
                ans.insert(i);
            }
        }
    }

    for (const auto& [s, b] : sensors) {
        if (b[1] == target_y) {
            ans.erase(b[0]);
        }
    }

    cout << ans.size() << endl;

    // Part 2
    vector<coord_t> to_check;
    for (const auto& [s, b] : sensors) {
        int d = dist(s, b) + 1;

        for (int i = 0; i <= d; i++) {
            coord_t ul = {s[0] - i, s[1] - d + i};
            coord_t ur = {s[0] + i, s[1] - d + i};
            coord_t bl = {s[0] - i, s[1] + d - i};
            coord_t br = {s[0] + i, s[1] + d - i};

            to_check.push_back(ul);
            to_check.push_back(ur);
            to_check.push_back(bl);
            to_check.push_back(br);
        }
    }

    cout << to_check.size() << endl;

    to_check.erase(remove_if(begin(to_check),
                             end(to_check),
                             [&](coord_t c) {
                                for (const auto& [s, b] : sensors) {
                                    size_t d = dist(s, b);

                                    if (dist(s, c) <= d) {
                                        return true;
                                    }
                                }
                                return !(c[0] >= 0 && c[0] < 4000000 && c[1] >= 0 && c[1] <= 4000000);
                            }), end(to_check));

    cout << to_check.size() << endl;

    for (const auto& c : to_check) {
        cout << c[0] << " " << c[1] << endl;
        cout << uint64_t(c[0]) * 4000000 + c[1] << endl;
    }

    return 0;
}