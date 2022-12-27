#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <queue>
#include <array>

using namespace std;

static constexpr size_t BUFF_SIZE = 14;

using buff_t = array<char, BUFF_SIZE>;

bool all_different(const buff_t& arr) {
    for (size_t i = 0; i < (arr.size() - 1); i++) {
        for (size_t j = i + 1; j < arr.size(); j++) {
            if (arr[i] == arr[j]) {
                return false;
            }
        }
    }

    return true;
}

int main() {
    ifstream fin("6.in");

    buff_t buf;
    size_t idx = 0;
    string line;
    uint64_t ans = 0;

    while (getline(fin, line)) {
        for (const auto& c : line) {
            buf[idx % BUFF_SIZE] = c;
            idx++;

            if (all_different(buf) && idx > BUFF_SIZE) {
                break;
            }
        }
    }

    cout << idx << endl;

    return 0;
}