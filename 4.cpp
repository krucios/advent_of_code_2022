#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <utility>
#include <vector>

using namespace std;

using range = pair<size_t, size_t>;

bool is_fully_contains(const range& first, const range& second) {
    return first.first <= second.first && first.second >= second.second;
}

bool has_overlap(const range& first, const range& second) {
    return (first.first >= second.first && first.first <= second.second)
        || (first.second >= second.first && first.second <= second.second);
}

range range_from_string(const string& s) {
    range ret;
    size_t pos = s.find('-');

    if (pos != std::string::npos) {
        string first_s  = s.substr(0, pos);
        string second_s = s.substr(pos + 1);

        ret.first  = stoi(first_s);
        ret.second = stoi(second_s);
    }

    return ret;
}

int main() {
    ifstream fin("4.in");

    string line;
    uint64_t ans_1 = 0;
    uint64_t ans_2 = 0;

    while (getline(fin, line)) {
        size_t pos = line.find(',');

        range first  = range_from_string(line.substr(0, pos));
        range second = range_from_string(line.substr(pos + 1));

        ans_1 += static_cast<uint64_t>(is_fully_contains(first, second) || is_fully_contains(second, first));
        ans_2 += static_cast<uint64_t>(has_overlap(first, second) || is_fully_contains(first, second) || is_fully_contains(second, first));
    }

    cout << ans_1 << " " << ans_2 << endl;

    return 0;
}