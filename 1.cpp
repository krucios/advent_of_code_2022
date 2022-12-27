#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>

using namespace std;

int main() {
    ifstream fin("1.in");

    string line;
    vector<uint64_t> cal;
    uint64_t acc = 0;

    while (getline(fin, line)) {
        if (line == "") {
            cal.push_back(acc);
            acc = 0;
        } else {
            acc += stoi(line);
        }
    }
    sort(begin(cal), end(cal), greater<>());

    cout << cal[0] + cal[1] + cal[2] << endl;

    return 0;
}