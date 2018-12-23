#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <functional>
#include <numeric>
#include <stack>

#define OPEN_GROUND '.'
#define TREES '|'
#define LUMBERYARD '#'

using namespace std;

class Grid {
private:
    vector<string> grid;

    unsigned int countSurrounding(const unsigned long &i, const unsigned long &j, const char &c) {
        unsigned int count = 0;

        bool up, down, left, right;

        up = i > 0;
        down = (i+1) < grid.size();
        left = j > 0;
        right = (j+1) < grid.at(i).size();

        if (up && left && grid.at(i-1).at(j-1) == c) count++;
        if (up && grid.at(i-1).at(j) == c) count++;
        if (up && right && grid.at(i-1).at(j+1) == c) count++;

        if (left && grid.at(i).at(j-1) == c) count++;
        if (right && grid.at(i).at(j+1) == c) count++;

        if (down && left && grid.at(i+1).at(j-1) == c) count++;
        if (down && grid.at(i+1).at(j) == c) count++;
        if (down && right && grid.at(i+1).at(j+1) == c) count++;

        return count;
    }
public:
    void readFile(const string &filename) {
        ifstream fin(filename);

        string line;

        while (getline(fin, line)) {
            grid.push_back(line);
        }

        fin.close();
    }

    void tick() {
        vector<string> newGrid = grid;

        for (unsigned long i = 0; i < grid.size(); i++) {
            for (unsigned long j = 0; j < grid.at(i).size(); j++) {
                switch (grid.at(i).at(j)) {
                    case OPEN_GROUND:
                        if (countSurrounding(i, j, TREES) >= 3) {
                            newGrid.at(i).at(j) = TREES;
                        }
                        break;
                    case TREES:
                        if (countSurrounding(i, j, LUMBERYARD) >= 3) {
                            newGrid.at(i).at(j) = LUMBERYARD;
                        }
                        break;
                    case LUMBERYARD:
                        if (!(countSurrounding(i, j, TREES) >= 1 && countSurrounding(i, j, LUMBERYARD) >= 1)) {
                            newGrid.at(i).at(j) = OPEN_GROUND;
                        }
                        break;
                    default:
                        cerr << "Unrecognized character: " << grid.at(i).at(j) << endl;
                        break;
                }
            }
        }

        grid = newGrid;
    }

    unsigned int getResourceValue() {
        unsigned int woodedAcres = 0, lumberyards = 0;

        for (const string &row : grid) {
            woodedAcres += count(row.begin(), row.end(), TREES);
            lumberyards += count(row.begin(), row.end(), LUMBERYARD);
        }

        return woodedAcres * lumberyards;
    }

    void print() {
        for_each(grid.begin(), grid.end(), [](const string &row) { cout << row << endl; });
        /*for (const string &row : grid) {
            cout << row << endl;
        }*/

        cout << endl;
    }

    void loop(unsigned int nLoops) {

        map<vector<string>, unsigned int> mem;
        unsigned int i = 0;

        for (i = 0; i < nLoops && mem.find(grid) == mem.end(); i++) {
            mem[grid] = i;
            tick();
        }

        // If it found a pattern...
        if (i < nLoops) {
            nLoops -= mem[grid];

            nLoops %= i - mem[grid];

            for (i = 0; i < nLoops; i++) {
                tick();
            }
        }
    }
};

void partOne() {
    Grid g;

    g.readFile("input");
    //g.print();

    for (int i = 0; i < 10; i++) {
        g.tick();
    }

    cout << "Solution to part one = " << g.getResourceValue() << endl;
}

void partTwo() {
    Grid g;

    g.readFile("input");

    g.loop(1000000000);
    // g.print();

    cout << "Solution to part two = " << g.getResourceValue() << endl;
}

int main() {
    cout << "--- Settlers of The North Pole ---" << endl;

    partOne();
    partTwo();

    return 0;
}