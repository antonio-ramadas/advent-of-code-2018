#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <list>

using namespace std;

int manhattamDistance(pair<int, int> lhs, pair<int, int> rhs) {
    return abs(lhs.first - rhs.first) + abs(lhs.second - rhs.second);
}

void partOne() {
    int x, y, maxX = -1, maxY = -1;
    char ignore;
    vector<pair<int, int> > coordinates;

    ifstream fin("input");
    while (fin >> x >> ignore >> y) {
        coordinates.emplace_back(x, y);
        maxX = max(maxX, x);
        maxY = max(maxY, y);
    }
    fin.close();

    int closestCoordinate = -1, coordinateDistance = -1;
    pair<int, int> currentCoordinates;
    vector<int> count(coordinates.size(), 0);

    for (y = 0; y <= maxY; y++) {
        for (x = 0; x <= maxX; x++) {
            closestCoordinate = -1;
            coordinateDistance = -1;
            currentCoordinates = make_pair(x, y);

            for (int i = 0; i < coordinates.size(); i++) {
                int distance = manhattamDistance(currentCoordinates, coordinates.at(i));

                if (coordinateDistance == -1 || distance <= coordinateDistance) {
                    if (distance == coordinateDistance && coordinateDistance != -1) {
                        // Same distance to two coordinates
                        closestCoordinate = -1;
                    } else {
                        closestCoordinate = i;
                    }

                    coordinateDistance = distance;
                }
            }

            if (closestCoordinate != -1) {
                if (x == 0 || y == 0 || x == maxX || y == maxY) {
                    count.at(closestCoordinate) = -1;
                } else if (count.at(closestCoordinate) != -1) {
                    count.at(closestCoordinate)++;
                }
            }
        }
    }

    cout << "Solution to part one = " << *max_element(count.begin(), count.end()) << endl;
}

void partTwo() {
    int x, y, maxX = -1, maxY = -1;
    char ignore;
    vector<pair<int, int> > coordinates;

    ifstream fin("input");
    while (fin >> x >> ignore >> y) {
        coordinates.emplace_back(x, y);
        maxX = max(maxX, x);
        maxY = max(maxY, y);
    }
    fin.close();

    pair<int, int> currentCoordinates;
    int sizeOfSafeRegion = 0, distance = 0;

    for (y = 0; y <= maxY; y++) {
        for (x = 0; x <= maxX; x++) {
            currentCoordinates = make_pair(x, y);
            distance = 0;

            for (const auto &coordinate : coordinates) {
                distance += manhattamDistance(currentCoordinates, coordinate);
            }

            if (distance < 10000) {
                sizeOfSafeRegion++;
            }
        }
    }

    cout << "Solution to part two = " << sizeOfSafeRegion << endl;
}

int main() {
    cout << "--- Chronal Coordinates ---" << endl;

    partOne();
    partTwo();

    return 0;
}
