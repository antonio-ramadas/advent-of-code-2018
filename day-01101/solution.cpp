#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <numeric>
#include <algorithm>

using namespace std;

void partOne() {
    vector<int> sequence = {3,7};
    vector<int> elves(sequence.size());

    // Assign the elves to the correct index
    iota(elves.begin(), elves.end(), 0);

    int currentDigitsSum = accumulate(sequence.begin(), sequence.end(), 0);

    int newPosition;
    unsigned int maxRecipes = 793031;
    unsigned int nFinalRecipes = 10;

    while (sequence.size() <= maxRecipes + nFinalRecipes) {
        for (const char &digit : to_string(currentDigitsSum)) {
            sequence.push_back(digit - '0');
        }

        for (int &elf : elves) {
            newPosition = elf + sequence.at(elf) + 1;
            newPosition %= sequence.size();

            currentDigitsSum += sequence.at(newPosition) - sequence.at(elf);

            elf = newPosition;
        }
    }

    string recipes;

    for (int recipeIndex = 0; recipeIndex < nFinalRecipes; recipeIndex++) {
        recipes += to_string(sequence.at(maxRecipes + recipeIndex));
    }

    cout << "Solution to part one = " << recipes << endl;
}

void partTwo() {
    // Not the most efficient solution (runs in about 4 seconds), but a very simple one
    // If sequence was a list, the algorithm would certainly be faster
    string sequence = "37";
    vector<int> elves(sequence.size());

    // Assign the elves to the correct index
    iota(elves.begin(), elves.end(), 0);

    int currentDigitsSum = 0;
    for (const char &digit : sequence) {
        currentDigitsSum += digit - '0';
    }

    int newPosition;
    string recipe = "793031";
    int lastIndex = 0;

    sequence += to_string(currentDigitsSum);

    while (sequence.find(recipe, lastIndex) == string::npos) {
        for (int &elf : elves) {
            newPosition = elf + sequence.at(elf) - '0' + 1;
            newPosition %= sequence.size();

            currentDigitsSum += (sequence.at(newPosition) - '0') - (sequence.at(elf) - '0');

            elf = newPosition;
        }

        lastIndex = max(0, (int) (sequence.size() - recipe.size()) - 5);

        sequence += to_string(currentDigitsSum);
    }

    cout << "Solution to part two = " << sequence.find(recipe, lastIndex) << endl;
}

int main() {
    cout << "--- Chocolate Charts ---" << endl;

    partOne();
    partTwo();

    return 0;
}
