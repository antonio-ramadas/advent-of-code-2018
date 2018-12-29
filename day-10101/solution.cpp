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
#include <unordered_set>
#include <iomanip>

using namespace std;

typedef pair<int, int> coordinates;

const coordinates UP = make_pair(-1, 0);
const coordinates DOWN = make_pair(1, 0);
const coordinates LEFT = make_pair(0, -1);
const coordinates RIGHT = make_pair(0, 1);

coordinates operator+(const coordinates &lhs, const coordinates &rhs) {
    return make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

enum class Regions {
    ROCKY,
    WET,
    NARROW
};

enum class Tools {
    CLIMBING_GEAR,
    TORCH,
    NEITHER
};

class Position {
private:
    coordinates position;
    unsigned int currentTime;
    unsigned int distanceToTarget;
    Tools tool;
public:
    Position(const coordinates &pos, const unsigned int &currTime, const unsigned int &distToTarget, const Tools &t) {
        position = pos;
        currentTime = currTime;
        distanceToTarget = distToTarget;
        tool = t;
    }

    coordinates getCoordinates() const {
        return position;
    }

    unsigned int getCurrentTime() const {
        return currentTime;
    }

    unsigned int getCost() const {
        return currentTime + distanceToTarget;
    }

    Tools getTool() const {
        return tool;
    }

    bool operator>(const Position &rhs) const {
        return getCost() > rhs.getCost();
    }
};

class Maze {
private:
    unsigned int depth;
    coordinates target;
    vector<vector<unsigned int>> maze;

    // For part 2
    vector<vector<Regions>> mazeRegions;

    unsigned int getDistanceToTarget(const coordinates &position) const {
        return abs(target.first - position.first) + abs(target.second - position.second);
    }

    list<Tools> getToolsForRegion(const Regions &reg) const {
        switch (reg) {
            case Regions::ROCKY:
                return {Tools::CLIMBING_GEAR, Tools::TORCH};
            case Regions::WET:
                return {Tools::CLIMBING_GEAR, Tools::NEITHER};
            case Regions::NARROW:
                return {Tools::TORCH, Tools::NEITHER};
        }
    }

    list<Position> getPositions(const Position &pos, const coordinates &newCoordinates) const {
        list<Position> positions;

        list<Tools> newRegionPossibleTools = getToolsForRegion(mazeRegions.at(newCoordinates.first).at(newCoordinates.second));
        list<Tools> currentRegionPossibleTools = getToolsForRegion(mazeRegions.at(pos.getCoordinates().first).at(pos.getCoordinates().second));

        unsigned int distanceToTarget = getDistanceToTarget(newCoordinates);

        for (const Tools &tool : newRegionPossibleTools) {
            // If the tool is not valid at the current position, then skip it
            if (find(currentRegionPossibleTools.begin(), currentRegionPossibleTools.end(), tool) == currentRegionPossibleTools.end()) {
                continue;
            }

            unsigned int time = pos.getCurrentTime() + 1;

            if (tool != pos.getTool()) {
                time += 7;
            }

            positions.push_back(Position(newCoordinates, time, distanceToTarget, tool));
        }

        return positions;
    }

    void addBottomRowIfNecessary(const coordinates &newCoordinates) {
        if (newCoordinates.first < maze.size()) {
            return;
        }

        vector<unsigned int> row = vector<unsigned int>(maze.at(0).size());
        vector<Regions> rowRegions = vector<Regions>(mazeRegions.at(0).size());

        vector<Regions> regions = {Regions::ROCKY, Regions::WET, Regions::NARROW};

        unsigned int x = maze.size();

        maze.push_back(row);
        mazeRegions.push_back(rowRegions);

        for (unsigned int y = 0; y < maze.at(x).size(); y++) {
            unsigned int geologicalIndex = 0;
            if (x == 0 || y == 0) {
                geologicalIndex = x * 16807 + y * 48271;
            } else if (x == target.first && y == target.second) {
                geologicalIndex = 0;
            } else {
                geologicalIndex = maze.at(x - 1).at(y) * maze.at(x).at(y - 1);
            }

            unsigned int erosionLevel = (geologicalIndex + depth) % 20183;

            maze.at(x).at(y) = erosionLevel;
            mazeRegions.at(x).at(y) = regions.at(erosionLevel % regions.size());
        }
    }

    void addRightColumnIfNecessary(const coordinates &newCoordinates) {
        if (newCoordinates.second < maze.at(0).size()) {
            return;
        }

        vector<Regions> regions = {Regions::ROCKY, Regions::WET, Regions::NARROW};

        for (unsigned int x = 0; x < maze.size(); x++) {
            unsigned int y = maze.at(x).size();

            unsigned int geologicalIndex = 0;
            if (x == 0 || y == 0) {
                geologicalIndex = x * 16807 + y * 48271;
            } else if (x == target.first && y == target.second) {
                geologicalIndex = 0;
            } else {
                geologicalIndex = maze.at(x - 1).at(y) * maze.at(x).at(y - 1);
            }

            unsigned int erosionLevel = (geologicalIndex + depth) % 20183;

            maze.at(x).push_back(erosionLevel);
            mazeRegions.at(x).push_back(regions.at(erosionLevel % regions.size()));
        }
    }

    list<Position> move(const Position &currentPosition) {
        list<Position> newPositions;
        coordinates newCoordinates;

        // Up
        if (currentPosition.getCoordinates().first > 0) {
            newCoordinates = currentPosition.getCoordinates() + UP;

            newPositions.splice(newPositions.begin(), getPositions(currentPosition, newCoordinates));
        }

        // Down
        newCoordinates = currentPosition.getCoordinates() + DOWN;
        addBottomRowIfNecessary(newCoordinates);
        newPositions.splice(newPositions.begin(), getPositions(currentPosition, newCoordinates));

        // Left
        if (currentPosition.getCoordinates().second > 0) {
            newCoordinates = currentPosition.getCoordinates() + LEFT;

            newPositions.splice(newPositions.begin(), getPositions(currentPosition, newCoordinates));
        }

        // Right
        newCoordinates = currentPosition.getCoordinates() + RIGHT;
        addRightColumnIfNecessary(newCoordinates);
        newPositions.splice(newPositions.begin(), getPositions(currentPosition, newCoordinates));

        return newPositions;
    }

public:
    void readFile(const string &filename) {
        ifstream fin(filename);

        fin.ignore(1000, ' ');
        fin >> depth;

        int x, y;
        fin.ignore(1000, ' ');
        fin >> x;
        fin.ignore(1);
        fin >> y;

        fin.close();

        target = make_pair(x, y);

        maze = vector<vector<unsigned int>>(target.first + 1, vector<unsigned int>(target.second + 1));
    }

    void compute() {
        for (unsigned int x = 0; x < maze.size(); x++) {
            for (unsigned int y = 0; y < maze.at(x).size(); y++) {
                unsigned int geologicalIndex = 0;
                if (x == 0 || y == 0) {
                    geologicalIndex = x * 16807 + y * 48271;
                } else if (x == target.first && y == target.second) {
                    geologicalIndex = 0;
                } else {
                    geologicalIndex = maze.at(x - 1).at(y) * maze.at(x).at(y - 1);
                }

                unsigned int erosionLevel = (geologicalIndex + depth) % 20183;

                maze.at(x).at(y) = erosionLevel;
            }
        }
    }

    unsigned int getTotalRiskLevelAtTarget() const {
        unsigned int totalRisk = 0;

        for (const vector<unsigned int> &row : maze) {
            for (const unsigned int &col : row) {
                totalRisk += col % 3;
            }
        }

        return totalRisk;
    }

    void convertMazeToRegionTypes() {
        mazeRegions = vector<vector<Regions>>(maze.size(), vector<Regions>(maze.at(0).size()));

        vector<Regions> regions = {Regions::ROCKY, Regions::WET, Regions::NARROW};

        for (unsigned int x = 0; x < maze.size(); x++) {
            for (unsigned int y = 0; y < maze.at(x).size(); y++) {

                mazeRegions.at(x).at(y) = regions.at(maze.at(x).at(y) % regions.size());
            }
        }
    }

    unsigned int getMinimumTimeToTarget() {
        unsigned int minimum = INT_MAX;

        priority_queue<Position, vector<Position>, greater<>> q;
        set<pair<coordinates, Tools>> visited;

        const coordinates startCoordinates = make_pair(0, 0);
        Position currentPosition = Position(startCoordinates, 0, target.first + target.second, Tools::TORCH);

        q.push(currentPosition);

        while (!q.empty()) {
            currentPosition = q.top();
            q.pop();

            pair<coordinates, Tools> currentVisit = make_pair(currentPosition.getCoordinates(), currentPosition.getTool());

            // If the cost is above the current minimum or the position has already been visited, ignore it
            if (!(currentPosition.getCost() < minimum && visited.find(currentVisit) == visited.end())) {
                continue;
            }

            // From now on, these coordinates with this tool will never be revisited
            visited.insert(currentVisit);

            if (currentPosition.getCoordinates() == target) {
                unsigned int cost = currentPosition.getCost();

                // "The target is always in a rocky region"
                if (currentPosition.getTool() == Tools::CLIMBING_GEAR) {
                    cost += 7;
                }

                minimum = min(minimum, cost);

                continue;
            }

            // Position not at target, but current best candidate

            for (const Position &pos : move(currentPosition)) {
                q.push(pos);
            }
        }

        return minimum;
    }
};

void partOne() {
    Maze m;

    m.readFile("input");

    m.compute();

    cout << "Solution to part one = " << m.getTotalRiskLevelAtTarget() << endl;
}

void partTwo() {
    Maze m;

    m.readFile("input");

    m.compute();
    m.convertMazeToRegionTypes();

    cout << "Solution to part two = " << m.getMinimumTimeToTarget() << endl;
}

int main() {
    cout << "--- Mode Maze ---" << endl;

    partOne();
    partTwo();

    return 0;
}
