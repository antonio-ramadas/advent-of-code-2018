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

using namespace std;

typedef pair<int, int> coordinates;

coordinates operator+(const coordinates &lhs, const coordinates &rhs) {
    return make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

class Reservoir {
private:
    vector<string> reservoir;
    coordinates springOfWater = make_pair(0, 500);

    bool boundedCoordinate(const coordinates &currentCoordinates) {
        bool boundedLeft = true, boundedRight = true;

        const string &currentLine = reservoir.at(currentCoordinates.first);
        const string &lineBelow = reservoir.at(currentCoordinates.first + 1);

        int j;

        for (j = currentCoordinates.second; j < currentLine.size() && currentLine.at(j) != '#' && boundedLeft; j++) {
            boundedLeft = lineBelow.at(j) == '#' || lineBelow.at(j) == '~';
        }
        boundedLeft = j < currentLine.size() && boundedLeft;

        for (j = currentCoordinates.second-1; j >= 0 && currentLine.at(j) != '#' && boundedRight && boundedLeft; j--) {
            boundedRight = lineBelow.at(j) == '#' || lineBelow.at(j) == '~';
        }
        boundedRight = j >= 0 && boundedRight;

        return boundedLeft && boundedRight;
    }

    void fillWater(const coordinates &currentCoordinates) {
        string &currentLine = reservoir.at(currentCoordinates.first);

        for (int j = currentCoordinates.second; j < currentLine.size() && currentLine.at(j) != '#'; j++) {
            currentLine.at(j) = '~';
        }

        for (int j = currentCoordinates.second-1; j >= 0 && currentLine.at(j) != '#'; j--) {
            currentLine.at(j) = '~';
        }
    }

    void fillDroppingWater(const coordinates &currentCoordinates) {
        string &currentLine = reservoir.at(currentCoordinates.first);
        const string &lineBelow = reservoir.at(currentCoordinates.first + 1);

        for (int j = currentCoordinates.second; j < currentLine.size() && currentLine.at(j) != '#' && lineBelow.at(j) != '.'; j++) {
            currentLine.at(j) = '|';
        }

        for (int j = currentCoordinates.second-1; j >= 0 && currentLine.at(j) != '#' && lineBelow.at(j) != '.'; j--) {
            currentLine.at(j) = '|';
        }
    }

    bool hasHolesToDecide(const coordinates &currentCoordinates) {
        string &currentLine = reservoir.at(currentCoordinates.first);
        const string &lineBelow = reservoir.at(currentCoordinates.first + 1);

        bool left = false, right = false;

        for (int j = currentCoordinates.second; j < currentLine.size() && currentLine.at(j) != '#' && lineBelow.at(j) != '|' && !left; j++) {
            if (lineBelow.at(j) == '.') {
                left = true;
            }
        }

        for (int j = currentCoordinates.second-1; j >= 0 && currentLine.at(j) != '#' && lineBelow.at(j) != '|' && !left && !right; j--) {
            if (lineBelow.at(j) == '.') {
                right = true;
            }
        }

        return left || right;
    }
public:
    void readFile(const string &filename) {
        ifstream fin(filename);
        string line;
        int anchor, start, end;
        int minX = INT_MAX, maxX = INT_MIN, maxY = 0, minY = INT_MAX;

        // Get limits
        while (getline(fin, line)) {
            if (sscanf(line.c_str(), "x=%d, y=%d..%d", &anchor, &start, &end)) {
                minX = min(minX, anchor);
                maxX = max(maxX, anchor);
                maxY = max(maxY, end);
                minY = min(minY, start);
            } else if (sscanf(line.c_str(), "y=%d, x=%d..%d", &anchor, &start, &end)) {
                minX = min(minX, start);
                maxX = max(maxX, end);
                maxY = max(maxY, anchor);
                minY = min(minY, anchor);
            } else {
                cerr << "Axis not identified: " << line << endl;
            }
        }

        fin.close();

        reservoir = vector<string>(maxY - minY + 2, string(maxX - minX + 3, '.'));
        springOfWater.second -= minX - 1;

        fin = ifstream(filename);

        // Fill reservoir with clay
        while (getline(fin, line)) {
            if (sscanf(line.c_str(), "x=%d, y=%d..%d", &anchor, &start, &end)) {
                for (; start <= end; start++) {
                    reservoir.at(start - minY + 1).at(anchor - minX + 1) = '#';
                }
            } else if (sscanf(line.c_str(), "y=%d, x=%d..%d", &anchor, &start, &end)) {
                for (; start <= end; start++) {
                    reservoir.at(anchor - minY + 1).at(start - minX + 1) = '#';
                }
            } else {
                cerr << "Axis not identified: " << line << endl;
            }
        }

        fin.close();
    }

    void print() {
        string firstRow = reservoir.at(0);
        firstRow.at(springOfWater.second) = '+';

        cout << firstRow << endl;

        for (unsigned long i = 1; i < reservoir.size(); i++) {
            cout << reservoir.at(i) << endl;
        }

        cout << endl;
    }

    void dropWater() {
        stack<coordinates> s;
        s.push(springOfWater);

        coordinates currentCoordinates;

        while (!s.empty()) {
            currentCoordinates = s.top();
            s.pop();

            if (currentCoordinates.first + 1 < reservoir.size()) {
                const char symbolBelow = reservoir.at(currentCoordinates.first + 1).at(currentCoordinates.second);

                if (symbolBelow == '.') {
                    // See what happens when the water falls and then decide
                    s.push(currentCoordinates);
                    s.push(currentCoordinates + make_pair(1, 0));
                } else if (symbolBelow == '|') {
                    // Just continue dropping
                    reservoir.at(currentCoordinates.first).at(currentCoordinates.second) = '|';
                } else { // if (symbolBelow == '~' || symbolBelow == '#') {
                    // Try to fill current level

                    if (boundedCoordinate(currentCoordinates)) {
                        fillWater(currentCoordinates);
                    } else if (!hasHolesToDecide(currentCoordinates)) {
                        fillDroppingWater(currentCoordinates);
                    } else {
                        // Add points to decide
                        s.push(currentCoordinates);

                        const string &currentLine = reservoir.at(currentCoordinates.first);
                        const string &lineBelow = reservoir.at(currentCoordinates.first + 1);

                        for (int j = currentCoordinates.second; j < currentLine.size() && currentLine.at(j) != '#'; j++) {
                            if (lineBelow.at(j) == '.') {
                                s.push(make_pair(currentCoordinates.first+1, j));
                                break;
                            }
                        }

                        for (int j = currentCoordinates.second-1; j >= 0 && currentLine.at(j) != '#'; j--) {
                            if (lineBelow.at(j) == '.') {
                                s.push(make_pair(currentCoordinates.first+1, j));
                                break;
                            }
                        }
                    }
                }
            } else {
                reservoir.at(currentCoordinates.first).at(currentCoordinates.second) = '|';
            }
        }
    }

    unsigned int countRunningWater() {
        unsigned int water = 0;

        for (unsigned long i = 1; i < reservoir.size(); i++) {
            const string &row = reservoir.at(i);
            water += count(row.begin(), row.end(), '~');
            water += count(row.begin(), row.end(), '|');
        }

        return water;
    }

    unsigned int countWater() {
        unsigned int water = 0;

        for (unsigned long i = 1; i < reservoir.size(); i++) {
            const string &row = reservoir.at(i);
            water += count(row.begin(), row.end(), '~');
        }

        return water;
    }
};

void partOne() {
    Reservoir reservoir;

    reservoir.readFile("input");
    reservoir.dropWater();
    // reservoir.print();

    cout << "Solution to part one = " << reservoir.countRunningWater() << endl;
}

void partTwo() {
    Reservoir reservoir;

    reservoir.readFile("input");
    reservoir.dropWater();
    // reservoir.print();

    cout << "Solution to part two = " << reservoir.countWater() << endl;
}

int main() {
    cout << "--- Reservoir Research ---" << endl;

    partOne();
    partTwo();

    return 0;
}