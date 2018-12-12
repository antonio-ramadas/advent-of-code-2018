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

void partTwo() {
    vector<vector<int> > grid(GRID_SIZE + 1, vector<int>(GRID_SIZE + 1, UNVISITED_CELL));
    vector<vector<int> > columnSum(GRID_SIZE + 1, vector<int>(GRID_SIZE + 1, 0));
    pair<int, int> coordinates;
    int maxPower = INT_MIN, bestWindowSize = 1;

    // Get power for each individual cell
    for (int y = 1; y <= GRID_SIZE; y++) {
        for (int x = 1; x <= GRID_SIZE; x++) {
            grid.at(y).at(x) = getPowerAt(grid, x, y);
        }
    }

    // Adapted from: https://www.geeksforgeeks.org/given-n-x-n-square-matrix-find-sum-sub-squares-size-k-x-k/
    // Could also have been solved using a different algorithm:
    // https://www.techiedelight.com/find-maximum-sum-submatrix-present-given-matrix/

    // For each possible size of the window
    for (int windowSize = 1; windowSize <= GRID_SIZE; windowSize++) {

        // Fill the columns
        for (int y = 1; y <= GRID_SIZE - windowSize + 1; y++) {
            for (int x = 1; x <= GRID_SIZE; x++) {
                columnSum.at(y).at(x) += grid.at(y + windowSize - 1).at(x);
            }
        }

        // Fill the rows
        for (int y = 1; y <= GRID_SIZE - windowSize + 1; y++) {

            int sum = 0;

            for (int x = 1; x <= windowSize; x++) {
                sum += columnSum.at(y).at(x);
            }

            if (sum > maxPower) {
                maxPower = sum;
                coordinates = make_pair(1, y);
                bestWindowSize = windowSize;
            }

            for (int x = 2; x <= GRID_SIZE - windowSize + 1; x++) {
                sum += columnSum.at(y).at(x + windowSize - 1) - columnSum.at(y).at(x-1);

                if (sum > maxPower) {
                    maxPower = sum;
                    coordinates = make_pair(x, y);
                    bestWindowSize = windowSize;
                }
            }
        }
    }

    cout << "Solution to part two = " << coordinates.first << "," << coordinates.second << "," << bestWindowSize << endl;
}

int main() {
    cout << "--- Chronal Charge ---" << endl;

    partOne();
    partTwo();

    return 0;
}
