#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

unsigned long partOne(string polymer) {
    unsigned int i = 0;

    while (i+1 < polymer.size()) {
        if (polymer.at(i) == polymer.at(i+1) + 32 || polymer.at(i) == polymer.at(i+1) - 32) {
            polymer.erase(i, 2);
            if (i > 0) {
                i--;
            }
        } else {
            i++;
        }
    }

    return polymer.size();
}

unsigned long partTwo(const string &polymer) {

    unsigned long smallestPolymerSize = polymer.size();
    string polymerCpy;

    for (char unit = 'A'; unit <= 'Z'; unit++) {
        polymerCpy = polymer;

        // A more efficient solution would be to make the loop by myself to avoid two traversals
        polymerCpy.erase(remove(polymerCpy.begin(), polymerCpy.end(), unit), polymerCpy.end());
        polymerCpy.erase(remove(polymerCpy.begin(), polymerCpy.end(), unit + 32), polymerCpy.end());

        smallestPolymerSize = min(smallestPolymerSize, partOne(polymerCpy));
    }

    return smallestPolymerSize;
}

int main() {
    cout << "--- Alchemical Reduction ---" << endl;

    string polymer;

    ifstream fin("input");
    getline(fin, polymer);
    fin.close();

    cout << "Solution to part one = " << partOne(polymer) << endl;
    cout << "Solution to part two = " << partTwo(polymer) << endl;

    return 0;
}
