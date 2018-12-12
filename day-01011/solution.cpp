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
    string newState, window;
    int potNumber = 0;

    for (int generationNumber = 1; generationNumber <= NUMBER_OF_GENERATIONS; generationNumber++) {
        newState = "";
        state = "....." + state + ".....";
        window = "....."; //state.substr(0, 5);
        potNumber -= 3;

        for (unsigned int i = 0; i < state.size() - window.size(); i++) {
            window.erase(window.begin());
            window += state.at(i + window.size());

            if (patterns.find(window) != patterns.end()) {
                newState += '#';
            } else {
                newState += '.';
            }
        }

        // newState.erase(0, newState.find_first_not_of('.'));       //prefixing spaces
        // newState.erase(newState.find_last_not_of('.')+1);

        state = newState;
    }

    int nPots = 0;
    for (int i = 0; i < newState.size(); i++) {
        if (newState.at(i) == '#')
            nPots += potNumber + i;
    }

    cout << "Solution to part one = " << nPots << endl;
}

int main() {
    cout << "--- Subterranean Sustainability ---" << endl;

    partOne();
    // partTwo();

    return 0;
}
