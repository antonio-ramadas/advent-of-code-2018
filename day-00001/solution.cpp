//
// Created by António Ramadas on 2018-12-01.
//

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

void partOne() {
    ifstream fin("input");

    string id;
    int exactlyTwo = 0, exactlyThree = 0;
    bool hasTwoSameLetters, hasThreeSameLetters;

    while (getline(fin, id)) {
        // A vector is also possible, but the alphabet used is not mentioned
        map<char, int> m;
        hasTwoSameLetters = false;
        hasThreeSameLetters = false;

        // These two are equivalent
        // for_each(id.begin(), id.end(), [&m](const char &letter) { m[letter]++; });
        for (const char &letter : id) { m[letter]++; }

        for (auto it : m) {
            hasTwoSameLetters = hasTwoSameLetters || it.second == 2;
            hasThreeSameLetters = hasThreeSameLetters || it.second == 3;
        }

        if (hasTwoSameLetters) {
            exactlyTwo++;
        }

        if (hasThreeSameLetters) {
            exactlyThree++;
        }
    }

    fin.close();

    cout << "Solution to part one = " << exactlyTwo * exactlyThree << endl;
}

int main() {
    cout << "--- Inventory Management System ---" << endl;

    partOne();

    return 0;
}
