#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <unordered_set>

#define NUMBER_OF_GENERATIONS 20

using namespace std;

void partOne() {
    string pattern, ignore, result, state;
    unordered_set<string> patterns;

    // Read
    ifstream fin("input");

    fin.ignore(1000, ':');
    fin >> state;

    while (fin >> pattern >> ignore >> result) {
        if (result == "#") {
            patterns.insert(pattern);
        }
    }
    fin.close();

    // Create useful variables
    list<char> generation(state.begin(), state.end());
    long nPots = count(generation.begin(), generation.end(), '#');
    list<char>::iterator initialState = generation.begin();

    for (int generationNumber = 1; generationNumber <= NUMBER_OF_GENERATIONS; generationNumber++) {
        // Go to the right

        // Go to the left


        nPots += count(generation.begin(), generation.end(), '#');
    }

    cout << "Solution to part one = " << nPots << endl;
}

int main() {
    cout << "--- Subterranean Sustainability ---" << endl;

    partOne();
    // partTwo();

    return 0;
}
