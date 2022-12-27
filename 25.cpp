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

int64_t char_to_int(char c) {
    int64_t ret;

    if (c == '2') {
        ret = 2;
    } else if (c == '1') {
        ret = 1;
    } else if (c == '0') {
        ret = 0;
    } else if (c == '-') {
        ret = -1;
    } else if (c == '=') {
        ret = -2;
    } else {
        cout << "Error" << endl;
        exit(1);
    }

    return ret;
}

char int_to_char(int64_t i) {
    return i ==  2 ? '2'
         : i ==  1 ? '1'
         : i ==  0 ? '0'
         : i == -1 ? '-'
         : i == -2 ? '='
         : 'X';
}

int64_t to_decimal(const string& snauf) {
    int64_t ans = 0;

    for (int64_t p = snauf.length() - 1, i = 0; p >= 0; p--, i++) {
        int64_t m = pow(5, p);

        ans += char_to_int(snauf[i]) * m;
    }

    return ans;
}

string to_snauf(int64_t d) {
    string rest = "";
    string ret = "";
    int64_t p = 0;
    int64_t base = 0;

    while (true) {
        int64_t base_p = pow(5, p);
        int64_t new_base = base + base_p;

        if (d <= new_base) {
            ret += "1";
            d -= base + 1;
            break;
        } else {
            base = new_base;
        }

        new_base = base + base_p;
        if (d <= new_base) {
            ret += "2";
            d -= base + 1;
            break;
        } else {
            base = new_base;
        }

        rest += "=";
        p++;
    }

    int idx = rest.length() - 1;
    while (d) {
        int rem = d % 5;

        rest[idx] = rem == 0 ? '='
                  : rem == 1 ? '-'
                  : rem == 2 ? '0'
                  : rem == 3 ? '1'
                  :            '2';

        idx--;
        d /= 5;
    };

    for (size_t i = 0; i < rest.length(); i++) {
        ret += rest[i];
    }

    return ret;
}

int main() {
    ifstream fin("25.in");

    string line;
    int64_t total = 0;
    while (getline(fin, line)) {
        int64_t d = to_decimal(line);
        total += d;
    }

    cout << total << " " << to_snauf(total) << endl;

    return 0;
}
