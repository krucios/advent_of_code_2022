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

int x = 1;
array<array<char, 40>, 6> matrix;

void advance_cycle(size_t& cycle) {
    int crt_col = (cycle - 1) % 40;
    int crt_row = (cycle - 1) / 40;

    bool is_lit = (crt_col - 1) <= x && x <= (crt_col + 1);

    matrix[crt_row][crt_col] = is_lit ? '#' : '.';

    cycle++;
}

int main() {
    ifstream fin("10.in");

    string line;
    size_t cycle = 1;

    while (getline(fin, line)) {
        stringstream ss(line);
        string command;

        ss >> command;

        if (command == "noop") {
            advance_cycle(cycle);
        } else {
            string operand;

            ss >> operand;

            advance_cycle(cycle);
            advance_cycle(cycle);

            x += stoi(operand);
        }
    }

    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[0].size(); j++) {
            cout << matrix[i][j];
        }
        cout << endl;
    }

    return 0;
}