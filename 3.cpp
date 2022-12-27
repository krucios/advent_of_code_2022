#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <array>

using namespace std;

constexpr static size_t MAX_ITEM = 53;

size_t char_to_num(const char c) {
    if ('a' <= c && c <= 'z') {
        return c - 'a' + 1;
    }

    return c - 'A' + 27;
}

int main() {
    ifstream fin("3.in");

    array<string, 3> lines;
    array<array<int, MAX_ITEM>, 3> elf;
    uint64_t ans = 0;

    while (getline(fin, lines[0])) {
        getline(fin, lines[1]);
        getline(fin, lines[2]);

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < MAX_ITEM; j++) {
                elf[i][j] = 0;
            }

            for (const char& c : lines[i]) {
                elf[i][char_to_num(c)]++;
            }
        }

        for (int i = 0; i < MAX_ITEM; i++) {
            if (elf[0][i] && elf[1][i] && elf[2][i]) {
                ans += i;
            }
        }
    }

    cout << ans << endl;

    return 0;
}