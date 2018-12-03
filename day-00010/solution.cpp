#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

void partOne() {
    ifstream fin("input");

    string line;
    unsigned int id, startX, startY, sizeX, sizeY, sum = 0;
    vector<vector<int> > fabric(1000, vector<int>(1000, 0));

    while (getline(fin, line)) {
        sscanf(line.c_str(), "#%d @ %d,%d: %dx%d", &id, &startX, &startY, &sizeX, &sizeY);

        for (unsigned int i = startX; i < startX + sizeX; i++) {
            for (unsigned int j = startY; j < startY + sizeY; j++) {
                if (fabric.at(i).at(j) == 1) {
                    sum++;
                }

                fabric.at(i).at(j)++;
            }
        }
    }

    fin.close();

    cout << "Solution to part one = " << sum << endl;
}

void partTwo() {
    ifstream fin("input");

    string line;
    unsigned int id, startX, startY, sizeX, sizeY;
    vector<vector<int> > fabric(1000, vector<int>(1000, -1));
    unordered_set<int> s;

    while (getline(fin, line)) {
        sscanf(line.c_str(), "#%d @ %d,%d: %dx%d", &id, &startX, &startY, &sizeX, &sizeY);
        s.insert(id);

        for (unsigned int i = startX; i < startX + sizeX; i++) {
            for (unsigned int j = startY; j < startY + sizeY; j++) {
                if (fabric.at(i).at(j) != -1) {
                    s.erase(fabric.at(i).at(j));
                    s.erase(id);
                }

                fabric.at(i).at(j) = id;
            }
        }
    }

    fin.close();

    cout << "Solution to part two = " << *(s.begin()) << endl;
}

int main() {
    cout << "--- No Matter How You Slice It ---" << endl;

    partOne();
    partTwo();

    return 0;
}
