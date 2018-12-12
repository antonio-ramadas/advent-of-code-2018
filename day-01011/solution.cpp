#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>

#define NUMBER_OF_GENERATIONS 20
#define LARGE_NUMBER_OF_GENERATIONS 50000000000

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

        int prevSize = newState.size();
        newState.erase(0, newState.find_first_not_of('.'));
        potNumber += prevSize - newState.size();
        newState.erase(newState.find_last_not_of('.')+1);

        state = newState;
    }

    int nPots = 0;
    for (int i = 0; i < state.size(); i++) {
        if (state.at(i) == '#')
            nPots += potNumber + i;
    }

    cout << "Solution to part one = " << nPots << endl;
}

void partTwo() {
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
    long long int potNumber = 0;
    unordered_map<string, pair<long long, unsigned long long> > generations;
    generations[state] = make_pair(potNumber, 0);

    for (long long generationNumber = 1; generationNumber <= LARGE_NUMBER_OF_GENERATIONS; generationNumber++) {
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

        int prevSize = newState.size();
        newState.erase(0, newState.find_first_not_of('.'));
        potNumber += prevSize - newState.size();
        newState.erase(newState.find_last_not_of('.')+1);

        state = newState;

        if (generations.find(newState) != generations.end()) {
            break;
        }

        cout << potNumber << " - " << generationNumber << ": " << state << endl;
        generations[state] = make_pair(potNumber, generationNumber);
    }

    // The rest of the following code is customised to this input
    // I left the previous print so you can see the repetition pattern forming up
    // Basically, after a certain point, the initial pot is moving one piece at a time
    // So, we can extrapolate that this growth will remain constant between consecutive generations
    //      and enter the formula above as shortcut

    long long delta = potNumber - generations[state].first;

    cout << endl << "Repetition found at generation " << generations[state].second << endl;
    cout << "Repetitive state: " << state << endl;
    cout << "Initial pot is at " << generations[state].first << endl;
    cout << "The pot moves by " << delta << " unit(s) by each generation" << endl;

    potNumber = (LARGE_NUMBER_OF_GENERATIONS - generations[state].second) * delta + generations[state].first;

    long long int nPots = 0;
    for (long long int i = 0; i < state.size(); i++) {
        if (state.at(i) == '#')
            nPots += potNumber + i;
    }

    cout << "Solution to part two = " << nPots << endl;
}

int main() {
    cout << "--- Subterranean Sustainability ---" << endl;

    partOne();
    partTwo();

    return 0;
}
