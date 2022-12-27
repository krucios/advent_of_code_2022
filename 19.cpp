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

using cost_t = array<size_t, 2>; // 0 - Ore, 1 - Clay

struct blueprint_t {
    size_t id;
    cost_t ore_robot_cost;
    cost_t clay_robot_cost;
    cost_t obsidian_robot_cost;
    cost_t geode_robot_cost;

    size_t max_ore_req;
    size_t max_clay_req;
    size_t max_obsidian_req;
};

struct state_t {
    size_t ore;
    size_t ore_robots;
    size_t clay;
    size_t clay_robots;
    size_t obsidian;
    size_t obsidian_robots;
    size_t geode;
    size_t geode_robots;

    void apply_production() {
        this->ore      += this->ore_robots;
        this->clay     += this->clay_robots;
        this->obsidian += this->obsidian_robots;
        this->geode    += this->geode_robots;
    }
};

blueprint_t parse_blueprint(const string& s) {
    blueprint_t ret;
    stringstream ss(s);

    ss.ignore(10);
    ss >> ret.id;

    ss.ignore(23);
    ss >> ret.ore_robot_cost[0];
    ret.ore_robot_cost[1] = 0;

    ss.ignore(28);
    ss >> ret.clay_robot_cost[0];
    ret.clay_robot_cost[1] = 0;

    ss.ignore(32);
    ss >> ret.obsidian_robot_cost[0];

    ss.ignore(9);
    ss >> ret.obsidian_robot_cost[1];

    ss.ignore(30);
    ss >> ret.geode_robot_cost[0];

    ss.ignore(9);
    ss >> ret.geode_robot_cost[1];

    ret.max_ore_req = max({ret.ore_robot_cost[0], ret.clay_robot_cost[0], ret.obsidian_robot_cost[0], ret.geode_robot_cost[0]});
    ret.max_clay_req = ret.obsidian_robot_cost[1];
    ret.max_obsidian_req = ret.geode_robot_cost[1];

    return ret;
}

void print_blueprint(const blueprint_t& b) {
    cout << "Blueprint [" << b.id << "]:" << endl;
    cout << "\tOre robot costs: " << b.ore_robot_cost[0] << " ore" << endl;
    cout << "\tClay robot costs: " << b.clay_robot_cost[0] << " ore" << endl;
    cout << "\tObsidian robot costs: " << b.obsidian_robot_cost[0] << " ore and " << b.obsidian_robot_cost[1] << " clay" << endl;
    cout << "\tGeode robot costs: " << b.geode_robot_cost[0] << " ore and " << b.geode_robot_cost[1] << " obsidian" << endl;
    cout << "\tMax requirements: ore - " << b.max_ore_req << ", clay - " << b.max_clay_req << ", obsidian - " << b.max_obsidian_req << endl << endl;
}

array<size_t, 32> best_so_far;

size_t geode_output(size_t m, state_t s, const blueprint_t& b) {
    if (m >= 32 || best_so_far[m] > s.geode) {
        return s.geode;
    }

    m++;
    best_so_far[m] = max(best_so_far[m], s.geode);

    size_t max_geodes = 0;
    state_t new_s = s;
    new_s.apply_production();

    if (s.ore >= b.geode_robot_cost[0] && s.obsidian >= b.geode_robot_cost[1]) {
        new_s.ore  -= b.geode_robot_cost[0];
        new_s.obsidian -= b.geode_robot_cost[1];
        new_s.geode_robots++;

        max_geodes = max(max_geodes, geode_output(m, new_s, b));;
    } else {
        if (s.ore >= b.ore_robot_cost[0] && s.ore_robots < b.max_ore_req) {
            new_s.ore -= b.ore_robot_cost[0];
            new_s.ore_robots++;

            max_geodes = max(max_geodes, geode_output(m, new_s, b));

            new_s.ore += b.ore_robot_cost[0];
            new_s.ore_robots--;
        }

        if (s.ore >= b.clay_robot_cost[0] && s.clay_robots < b.max_clay_req) {
            new_s.ore -= b.clay_robot_cost[0];
            new_s.clay_robots++;

            max_geodes = max(max_geodes, geode_output(m, new_s, b));

            new_s.ore += b.clay_robot_cost[0];
            new_s.clay_robots--;
        }

        if (s.ore >= b.obsidian_robot_cost[0] && s.clay >= b.obsidian_robot_cost[1] && s.obsidian_robots < b.max_obsidian_req) {
            new_s.ore -= b.obsidian_robot_cost[0];
            new_s.clay -= b.obsidian_robot_cost[1];
            new_s.obsidian_robots++;

            max_geodes = max(max_geodes, geode_output(m, new_s, b));

            new_s.ore += b.obsidian_robot_cost[0];
            new_s.clay += b.obsidian_robot_cost[1];
            new_s.obsidian_robots--;
        }

        s.apply_production();
        max_geodes = max(max_geodes, geode_output(m, s, b));
    }

    return max_geodes;
}

int main() {
    ifstream fin("19.in");

    string line;

    vector<blueprint_t> blueprints;

    while (getline(fin, line)) {
        blueprint_t b = parse_blueprint(line);
        blueprints.push_back(b);

        print_blueprint(b);
    }

    uint64_t ans = 0;
    for (const auto& b : blueprints) {
        state_t s;
        s.ore = 0;
        s.ore_robots = 1;
        s.clay = 0;
        s.clay_robots = 0;
        s.obsidian = 0;
        s.obsidian_robots = 0;
        s.geode = 0;
        s.geode_robots = 0;

        for (auto& e : best_so_far) { e = 0; }

        uint64_t output = geode_output(0, s, b);
        ans += output * b.id;
        cout << "For blueprint " << b.id << " max globes is " << output << endl;
    }

    cout << ans << endl;

    return 0;
}