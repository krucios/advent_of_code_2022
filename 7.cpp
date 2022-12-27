#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <utility>
#include <vector>
#include <stack>
#include <map>

using namespace std;

using file_t  = pair<string, uint64_t>;
using files_t = vector<file_t>;
using fs_t    = map<string, files_t>;

bool is_command(const string& s) {
    return s[0] == '$';
}

bool has_dirs(const fs_t& fs) {
    for (const auto& [key, value] : fs) {
        for (const auto& e : value) {
            if (e.second == 0) {
                return true;
            }
        }
    }
    return false;
}

void print_fs(const fs_t& fs) {
    for (const auto& [key, value] : fs) {
        cout << key << ": ";
        for (const auto& e : value) {
            cout << "\t" << e.second << " " << e.first << endl;;
        }
    }
}

string get_absolute_path(stack<string> s) {
    string ret;

    while (s.size()) {
        ret.insert(0, s.top());
        s.pop();
    }

    return ret;
}

uint64_t dir_size(const files_t& f) {
    uint64_t ret = 0;

    for (const auto& e : f) {
        ret += e.second;
    }

    return ret;
}

int main() {
    ifstream fin("7.in");

    string line;

    stack<string> cur_dir;
    fs_t fs;

    while (getline(fin, line)) {
        stringstream ss(line);

        if (is_command(line)) {
            string dummy;
            string command;
            string dir;

            ss >> dummy >> command;

            if (command == "cd") {
                ss >> dir;

                if (dir == "..") {
                    cur_dir.pop();
                } else if (dir == "/") {
                    cur_dir = {};
                    cur_dir.push(dir);
                } else {
                    cur_dir.push(dir + '/');
                }
            }
        } else {
            string size;
            string name;

            ss >> size >> name;

            string cur_path = get_absolute_path(cur_dir);
            if (size == "dir") {
                fs[cur_path].push_back(make_pair(cur_path + name + '/', 0));
            } else {
                fs[cur_path].push_back(make_pair(name, stoi(size)));
            }
        }
    }

    print_fs(fs);

    // Unfold directories
    size_t iteration = 0;
    while (has_dirs(fs)) {
        fs_t new_fs;

        cout << "Unfold iteration " << ++iteration << " ..." << endl;

        for (auto& [key, value] : fs) {
            files_t new_files;

            for (const auto& e : value) {
                if (e.second == 0) {
                    if (key == e.first) cout << "Gotcha!: " << key << endl;

                    new_files.insert(end(new_files), begin(fs[e.first]), end(fs[e.first]));
                } else {
                    new_files.push_back(e);
                }
            }

            new_fs[key] = new_files;

            cout << key << ": old files size " << fs[key].size() << ", new files size " << new_files.size() << endl;
        }

        fs = new_fs;
    }

    uint64_t ans = 0;
    for (auto& [key, value] : fs) {
        uint64_t s = dir_size(value);

        if (s <= 100000) {
            ans += s;
        }
    }

    cout << ans << endl;

    uint64_t total_size = dir_size(fs["/"]);
    uint64_t to_delete = total_size - 40000000;

    cout << "To delete: " << to_delete << endl;

    uint64_t min_delete = total_size;
    for (auto& [key, value] : fs) {
        uint64_t s = dir_size(value);

        if (s >= to_delete && s < min_delete) {
            min_delete = s;
        }
    }

    cout << min_delete << endl;

    return 0;
}