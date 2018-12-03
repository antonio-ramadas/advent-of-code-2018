#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <list>

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

void partTwo() {
    ifstream fin("input");

    string currId, solution;
    list<string> l;

    while (getline(fin, currId)) {
       for (const string &id : l) {
           // Currently, a solution is constantly built (which may be expensive)
           // Another solution would be to use a flag to mark compatible IDs and only then build the string
           solution = "";

           // I am assuming here that a missing character is not the same as having one different character
           // For example, should `abc` and `abcd` count?
           if (id.size() != currId.size()) {
               continue;
           }

           for (unsigned int i = 0; i < currId.size(); i++) {
               if (currId.at(i) == id.at(i)) {
                   solution += currId.at(i);
               }
           }

           if (solution.size() == currId.size() - 1) {
               cout << "Solution to part two = " << solution << endl;
               fin.close();
               return;
           }
       }

       l.push_back(currId);
    }

    fin.close();
}

int main() {
    cout << "--- Inventory Management System ---" << endl;

    partOne();
    partTwo();

    return 0;
}
