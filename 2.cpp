#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>

using namespace std;

enum Shape {
    rock     = 1,
    paper    = 2,
    scissors = 3
};

enum Outcome {
    win  = 6,
    draw = 3,
    lose = 0
};

Shape shape_from_char(const char c) {
    if (c == 'A') {
        return rock;
    }

    if (c == 'B') {
        return paper;
    }

    return scissors;
}

Outcome outcome_from_char(const char c) {
    if (c == 'X') {
        return lose;
    }

    if (c == 'Y') {
        return draw;
    }

    return win;
}

size_t game_round(const Shape opponent, const Shape mine) {
    if (opponent == mine) {
        return 3;
    }

    if ((mine == rock     && opponent == scissors)
    ||  (mine == paper    && opponent == rock    )
    ||  (mine == scissors && opponent == paper   )) {
        return 6;
    }

    return 0;
}

size_t calculate_my_shape(const Shape opponent, const Outcome outcome) {
    if (outcome == draw) {
        return opponent;
    }

    if (outcome == lose) {
        if (opponent == rock) {
            return scissors;
        } else if (opponent == paper) {
            return rock;
        } else {
            return paper;
        }
    }

    if (opponent == rock) {
        return paper;
    } else if (opponent == paper) {
        return scissors;
    } else {
        return rock;
    }
}

int main() {
    ifstream fin("2.in");

    string line;
    uint64_t ans = 0;

    while (getline(fin, line)) {
        auto opponent = shape_from_char(line[0]);
        auto outcome  = outcome_from_char(line[2]);

        ans += outcome + calculate_my_shape(opponent, outcome);
    }

    cout << ans << endl;

    return 0;
}