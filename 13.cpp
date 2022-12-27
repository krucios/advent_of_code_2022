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

bool is_list(const string& s) {
    return s[0] == '[';
}

string extract_term(string& s) {
    size_t start = s.find('[');
    size_t end = 0;
    size_t depth = 0;

    for (size_t i = start; i < s.length(); i++) {
        if (s[i] == '[') {
            depth++;
        } else if (s[i] == ']') {
            depth--;
        }

        if (depth == 0) {
            end = i;
            break;
        }
    }

    string ret = s.substr(start, end - start + 1);
    s.erase(start, end - start + 1);
    return ret;
}

vector<string> parse(string s) {
    vector<string> terms;

    if (is_list(s)) {
        s.erase(0, 1);
        s.erase(s.length() - 1, 1);
    }

    while (s.length() != 0) {
        if (is_list(s)) {
            terms.push_back(extract_term(s));
        } else { // Number
            size_t amount = 0;
            while (isdigit(s[amount])) {
                amount++;
            }
            terms.push_back(s.substr(0, amount));
            s.erase(0, amount);
        }

        if (s.length() && s[0] == ',') {
            s.erase(0, 1);
        }
    }

    return terms;
}

int is_in_order_rec(const string& p1, const string& p2) {
    vector<string> t1 = parse(p1);
    vector<string> t2 = parse(p2);

    int ret = 0;

    for (size_t i = 0; i < t1.size(); i++) {
        if (t2.size() == i) { // Right list runs out of items first
            return -1;
        }

        string e1 = t1[i];
        string e2 = t2[i];

        if (is_list(e1) || is_list(e2)) { // Start recursion
            if (!is_list(e1)) {
                e1 = "["s + e1 + "]";
            }

            if (!is_list(e2)) {
                e2 = "["s + e2 + "]";
            }

            ret = is_in_order_rec(e1, e2);

            if (ret) {
                return ret;
            }
        } else { // Both numbers
            ret = stoi(e1) < stoi(e2) ? 1
                : stoi(e1) > stoi(e2) ? -1
                : 0;

            if (ret) {
                return ret;
            }
        }
    }

    if (t2.size() > t1.size()) {
        return 1;
    } else {
        return 0;
    }
}

bool is_in_order(const string& s1, const string& s2) {
    return is_in_order_rec(s1, s2) == 1;
}

int main() {
    ifstream fin("13.in");

    string line;
    string pkt_1;
    string pkt_2;
    size_t idx = 1;
    size_t ans = 0;

    vector<string> packets;

    while (getline(fin, line)) {
        if (line != "") {
            pkt_1 = line;
            getline(fin, pkt_2);
        } else {
            packets.push_back(pkt_1);
            packets.push_back(pkt_2);

            if (is_in_order(pkt_1, pkt_2)) {
                ans += idx;
            }
            idx++;
        }
    }

    cout << ans << endl;

    packets.push_back("[[2]]");
    packets.push_back("[[6]]");

    sort(begin(packets), end(packets), [](const string& s1, const string& s2) {
        return is_in_order(s1, s2);
    });

    for (const auto& e : packets) {
        cout << e << endl;
    }

    size_t ans_2 = 1;
    for (size_t i = 0; i < packets.size(); i++) {
        if (packets[i] == "[[2]]") { ans_2 *= (i + 1); }
        if (packets[i] == "[[6]]") { ans_2 *= (i + 1); }
    }

    cout << ans_2 << endl;

    return 0;
}