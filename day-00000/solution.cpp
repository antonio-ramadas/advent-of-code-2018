//
// Created by António Ramadas on 2018-12-01.
//

#include <iostream>
#include <fstream>
#include <set>
#include <list>

using namespace std;

void partOne() {
    ifstream fin("input");

    int sum = 0, current = 0;

    while (fin >> current) {
        sum += current;
    }

    fin.close();

    cout << "Solution to part one = " << sum << endl;
}

void partTwo() {
    ifstream fin("input");

    list<int> l;
    int current = 0;

    while (fin >> current) {
        l.push_back(current);
    }

    fin.close();

    int sum = 0;
    // unordered_set would be more time efficient, but set is more efficient in terms of memory
    // Either way, the input case is small enough for this decision to have a marginal impact
    set<int> s;
    auto it = l.begin();

    while (true) {
        sum += *it;

        if (s.find(sum) != s.end()) {
            cout << "Solution to part two = " << sum << endl;
            return;
        }

        s.insert(sum);

        it++;
        // Custom implementation of a circular list
        if (it == l.end()) {
            it = l.begin();
        }
    }
}

int main() {
    cout << "--- Chronal Calibration ---" << endl;

    partOne();
    partTwo();

    return 0;
}
