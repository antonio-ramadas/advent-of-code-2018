#include <iostream>
#include <fstream>
#include <vector>
#include <list>

using namespace std;

// The range of the power of each cell is -5 <= power <=4
#define UNVISITED_CELL -10
#define INPUT 5153
#define GRID_SIZE 300

vector<int> dx = {1, 0, -1,  0, -1, 1, -1,  1, 0};
vector<int> dy = {0, 1,  0, -1, -1, 1,  1, -1, 0};

int getPowerAt(vector<vector<int> > &grid, const int &x, const int &y) {
    int &power = grid.at(y).at(x);

    if (power == UNVISITED_CELL) {
        int rackId = x + 10;
        power = rackId * y;
        power += INPUT;
        power *= rackId;
        power /= 100;
        power %= 10;
        power -= 5;
    }

    return power;
}

int getMatrixPowerAt(vector<vector<int> > &grid, const int &centerX, const int &centerY) {
    int power = 0;

    for (unsigned int i = 0; i < dx.size(); i++) {
        power += getPowerAt(grid, centerX + dx.at(i), centerY + dy.at(i));
    }

    return power;
}

void partOne() {
    int maxPower = INT_MIN, power;
    pair<int, int> coordinates;
    vector<vector<int> > grid(GRID_SIZE + 1, vector<int>(GRID_SIZE + 1, UNVISITED_CELL));

    // The grid starts at 1,1
    for (int x = 2; x < GRID_SIZE; x++) {
        for (int y = 2; y < GRID_SIZE; y++) {
            power = getMatrixPowerAt(grid, x, y);
            if (power > maxPower) {
                maxPower = power;
                coordinates = make_pair(x - 1, y - 1);
            }
        }
    }

    cout << "Solution to part one = " << coordinates.first << "," << coordinates.second << endl;
}

int main() {
    cout << "--- Chronal Charge ---" << endl;

    partOne();

    return 0;
}
