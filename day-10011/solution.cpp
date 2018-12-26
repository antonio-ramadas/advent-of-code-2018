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
#include <unordered_map>
#include <iomanip>

using namespace std;

typedef pair<int, int> coordinates;

#define NORTH make_pair(-1, 0)
#define SOUTH make_pair(1, 0)
#define WEST make_pair(0, 1)
#define EAST make_pair(0, -1)

coordinates operator+(const coordinates &lhs, const coordinates &rhs) {
    return make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

coordinates& operator+=(coordinates &lhs, const coordinates &rhs) {
    lhs.first += rhs.first;
    lhs.second += rhs.second;
    return lhs;
}

class Maze {
private:
    coordinates startCoordinates = make_pair(0, 0);
    map<coordinates, set<coordinates>> connections;
    string rawInstructions;

    void addConnection(const coordinates &position, const coordinates &direction) {
        connections[position].insert(position + direction);
        connections[position + direction].insert(position);
    }
public:
    explicit Maze(const string &instructions) {
        rawInstructions = instructions;
    }

    // Time complexity is linear
    set<coordinates> readInstructionsImproved(const set<coordinates> &initialPositions, unsigned long &index) {
        set<coordinates> tmpCurrentPositions, currentPositions = initialPositions;

        set<coordinates> positions;

        for (; index < rawInstructions.size(); index++) {
            switch (rawInstructions.at(index)) {
                case 'N':
                    for (const coordinates &coord : currentPositions) {
                        addConnection(coord, NORTH);
                        tmpCurrentPositions.insert(coord + NORTH);
                    }
                    currentPositions = tmpCurrentPositions;
                    tmpCurrentPositions.clear();
                    break;
                case 'S':
                    for (const coordinates &coord : currentPositions) {
                        addConnection(coord, SOUTH);
                        tmpCurrentPositions.insert(coord + SOUTH);
                    }
                    currentPositions = tmpCurrentPositions;
                    tmpCurrentPositions.clear();
                    break;
                case 'W':
                    for (const coordinates &coord : currentPositions) {
                        addConnection(coord, WEST);
                        tmpCurrentPositions.insert(coord + WEST);
                    }
                    currentPositions = tmpCurrentPositions;
                    tmpCurrentPositions.clear();
                    break;
                case 'E':
                    for (const coordinates &coord : currentPositions) {
                        addConnection(coord, EAST);
                        tmpCurrentPositions.insert(coord + EAST);
                    }
                    currentPositions = tmpCurrentPositions;
                    tmpCurrentPositions.clear();
                    break;
                case '(':
                    index++;
                    positions = readInstructionsImproved(currentPositions, index);
                    break;
                case '|':
                    positions.insert(currentPositions.begin(), currentPositions.end());
                    currentPositions = initialPositions;
                    break;
                case ')':
                    //index++;
                    positions.insert(currentPositions.begin(), currentPositions.end());
                    currentPositions = initialPositions;
                    return positions;
                default:
                    // Ignore some of the characters (^ and $)
                    break;
            }
        }

        return positions;
    }

    unsigned int getNumberOfDoorsToMostDistantRoom() {
        unsigned int numberOfDoors = 0;
        set<coordinates> visited;
        queue<pair<coordinates, unsigned int>> toVisit;
        toVisit.emplace(startCoordinates, 0);

        while (!toVisit.empty()) {
            coordinates currentCoordinates = toVisit.front().first;
            unsigned int currentDistance = toVisit.front().second;
            toVisit.pop();

            if (visited.find(currentCoordinates) != visited.end()) {
                continue;
            }

            visited.insert(currentCoordinates);

            numberOfDoors = max(numberOfDoors, currentDistance);

            for (const coordinates &connection : connections[currentCoordinates]) {
                toVisit.emplace(connection, currentDistance + 1);
            }
        }

        return numberOfDoors;
    }

    unsigned int countNumberOfRoomsMoreDistantThan(const unsigned int &distance) {
        unsigned int numberOfRooms = 0;
        set<coordinates> visited;
        queue<pair<coordinates, unsigned int>> toVisit;
        toVisit.emplace(startCoordinates, 0);

        while (!toVisit.empty()) {
            coordinates currentCoordinates = toVisit.front().first;
            unsigned int currentDistance = toVisit.front().second;
            toVisit.pop();

            if (visited.find(currentCoordinates) != visited.end()) {
                continue;
            }

            visited.insert(currentCoordinates);

            if (currentDistance >= distance) {
                numberOfRooms++;
            }

            for (const coordinates &connection : connections[currentCoordinates]) {
                toVisit.emplace(connection, currentDistance + 1);
            }
        }

        return numberOfRooms;
    }

    coordinates getStartCoordinates() const {
        return startCoordinates;
    }
};

void partOne() {
    ifstream fin("input");
    string line;
    getline(fin, line);
    fin.close();

    Maze m(line);
    unsigned long index = 0;

    m.readInstructionsImproved({m.getStartCoordinates()}, index);
    cout << "Solution to part one = " << m.getNumberOfDoorsToMostDistantRoom() << endl;
}

void partTwo() {
    ifstream fin("input");
    string line;
    getline(fin, line);
    fin.close();

    Maze m(line);
    unsigned long index = 0;

    m.readInstructionsImproved({m.getStartCoordinates()}, index);
    cout << "Solution to part two = " << m.countNumberOfRoomsMoreDistantThan(1000) << endl;
}

int main() {
    cout << "--- A Regular Map ---" << endl;

    partOne();
    partTwo();

    return 0;
}
