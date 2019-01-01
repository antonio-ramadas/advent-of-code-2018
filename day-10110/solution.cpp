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

class ThreeDCoordinates {
public:
    int x, y, z;

    ThreeDCoordinates() {
        x = y = z = 0;
    }

    ThreeDCoordinates(const int &x, const int &y, const int &z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    unsigned int getDistanceToOrigin() const {
        return abs(x) + abs(y) + abs(z);
    }
};

class Nanobot {
private:
    ThreeDCoordinates coordinates;
    int radius;
public:
    Nanobot() {
        radius = INT_MIN;
    }

    Nanobot(const ThreeDCoordinates &coordinates, const int &radius) {
        this->coordinates = coordinates;
        this->radius = radius;
    }

    int getDistanceTo(const ThreeDCoordinates &coord) const {
        return abs(coordinates.x - coord.x) + abs(coordinates.y - coord.y) + abs(coordinates.z - coord.z);
    }

    int getRadius() const {
        return radius;
    }

    ThreeDCoordinates getCoordinates() const {
        return coordinates;
    }
};

class Cube {
private:
    ThreeDCoordinates coordinates;
    int edgeSize;
    list<Nanobot> nanobots;

    ThreeDCoordinates getClosestCoordinateToOrigin() const {
        ThreeDCoordinates tdc;

        tdc.x = abs(coordinates.x) < abs(coordinates.x + edgeSize) ? coordinates.x : coordinates.x + edgeSize;
        tdc.y = abs(coordinates.y) < abs(coordinates.y + edgeSize) ? coordinates.y : coordinates.y + edgeSize;
        tdc.z = abs(coordinates.z) < abs(coordinates.z + edgeSize) ? coordinates.z : coordinates.z + edgeSize;

        return tdc;
    }

    int outsideDistance(const int &coord, const int &lhs, const int &rhs) const {
        if (coord < lhs) {
            return lhs - coord;
        }

        if (coord > rhs) {
            return coord - rhs;
        }

        return 0;
    }
public:
    Cube(const ThreeDCoordinates &tdc, const int &edge, const list<Nanobot> &nb) {
        coordinates = tdc;
        edgeSize = edge;
        nanobots = nb;
    }

    bool operator<(const Cube &rhs) const {
        if (nanobots.size() != rhs.nanobots.size()) {
            return nanobots.size() < rhs.nanobots.size();
        }

        ThreeDCoordinates lhsTdc = getClosestCoordinateToOrigin();
        ThreeDCoordinates rhsTdc = rhs.getClosestCoordinateToOrigin();

        if (lhsTdc.getDistanceToOrigin() != rhsTdc.getDistanceToOrigin()) {
            return lhsTdc.getDistanceToOrigin() < rhsTdc.getDistanceToOrigin();
        }

        return edgeSize < rhs.edgeSize;
    }

    list<Cube> split() const {
        // Splits in eight like in an octree

        int newEdgeSize = edgeSize / 2;
        ThreeDCoordinates tdc;

        list<Cube> splits;

        for (int i = 0; i <= 1; i++) {
            for (int j = 0; j <= 1; j++) {
                for (int k = 0; k <= 1; k++) {
                    tdc.x = coordinates.x + i * newEdgeSize;
                    tdc.y = coordinates.y + j * newEdgeSize;
                    tdc.z = coordinates.z + k * newEdgeSize;

                    list<Nanobot> nbs;

                    for (const Nanobot &nb : nanobots) {
                        int distance = 0;

                        distance += outsideDistance(nb.getCoordinates().x, tdc.x, tdc.x + newEdgeSize - 1);
                        distance += outsideDistance(nb.getCoordinates().y, tdc.y, tdc.y + newEdgeSize - 1);
                        distance += outsideDistance(nb.getCoordinates().z, tdc.z, tdc.z + newEdgeSize - 1);

                        if (distance <= nb.getRadius()) {
                            nbs.push_back(nb);
                        }
                    }

                    splits.push_back(Cube(tdc, newEdgeSize, nbs));
                }
            }
        }

        return splits;
    }

    ThreeDCoordinates getCoordinates() const {
        return coordinates;
    }

    int getEdgeSize() const {
        return edgeSize;
    };

    bool empty() const {
        return nanobots.empty();
    }
};

class Cavern {
private:
    list<Nanobot> nanobots;
public:
    void readFile(const string &filename) {
        ifstream fin(filename);

        string line;
        int x, y, z, radius;

        while (getline(fin, line)) {
            sscanf(line.c_str(), "pos=<%d,%d,%d>, r=%d", &x, &y, &z, &radius);

            nanobots.push_back(Nanobot(ThreeDCoordinates(x, y, z), radius));
        }

        fin.close();
    }

    unsigned int countNanobotsInRangeOfStrongest() {
        Nanobot strongestNB;

        for (const Nanobot &nb : nanobots) {
            if (nb.getRadius() > strongestNB.getRadius()) {
                strongestNB = nb;
            }
        }

        unsigned int count = 0;

        for (const Nanobot &nb : nanobots) {
            if (nb.getDistanceTo(strongestNB.getCoordinates()) <= strongestNB.getRadius()) {
                count++;
            }
        }

        return count;
    }

    unsigned int getShortestDistanceToBestPoint() {
        pair<int, int> x, y, z = make_pair(INT_MAX, INT_MIN);
        x = y = z;
        
        for (const Nanobot &nb : nanobots) {
            x.first = min(x.first, nb.getCoordinates().x - nb.getRadius());
            x.second = max(x.second, nb.getCoordinates().x + nb.getRadius());

            y.first = min(y.first, nb.getCoordinates().y - nb.getRadius());
            y.second = max(y.second, nb.getCoordinates().y + nb.getRadius());

            z.first = min(z.first, nb.getCoordinates().z - nb.getRadius());
            z.second = max(z.second, nb.getCoordinates().z + nb.getRadius());
        }

        int edgeSize = x.second - x.first;
        edgeSize = max(edgeSize, y.second - y.first);
        edgeSize = max(edgeSize, z.second - z.first);

        edgeSize++;

        priority_queue<Cube> q;

        q.push(Cube(ThreeDCoordinates(x.first, y.first, z.first), edgeSize, nanobots));

        while (!q.empty()) {
            Cube cb = q.top();
            q.pop();

            if (cb.getEdgeSize() == 1) {
                return cb.getCoordinates().getDistanceToOrigin();
            } else {
                for (const Cube &smallCube : cb.split()) {
                    if (!smallCube.empty()) {
                        q.push(smallCube);
                    }
                }
            }
        }

        // No intersections (will never get here)
        return INT_MAX;
    }
};

void partOne() {
    Cavern c;

    c.readFile("input");

    cout << "Solution to part one = " << c.countNanobotsInRangeOfStrongest() << endl;
}

void partTwo() {
    Cavern c;

    c.readFile("input");

    cout << "Solution to part two = " << c.getShortestDistanceToBestPoint() << endl;
}

int main() {
    cout << "--- Experimental Emergency Teleportation ---" << endl;

    partOne();
    partTwo();

    return 0;
}
