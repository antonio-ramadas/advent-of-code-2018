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

class Coordinates {
private:
    int x, y, z, k;
public:
    Coordinates(const int &x, const int &y, const int &z, const int &k) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->k = k;
    }

    int getDistance(const Coordinates &coord) const {
        return abs(x - coord.x) + abs(y - coord.y) + abs(z - coord.z) + abs(k - coord.k);
    }
};

class Sky {
private:
    list<list<Coordinates>> constellations;

public:
    void createConstellations(const string &filename) {
        ifstream fin(filename);

        int x, y, z, k;
        char ignore;

        while (fin >> x >> ignore >> y >> ignore >> z >> ignore >> k) {
            Coordinates coordinates(x, y, z, k);

            list<list<list<Coordinates>>::iterator> matchedConstellations;

            for (auto constellation = constellations.begin(); constellation != constellations.end(); constellation++) {
                bool matched = false;

                for (auto point = constellation->begin(); point != constellation->end() && !matched; point++) {
                    if (coordinates.getDistance(*point) <= 3) {
                        matched = true;
                    }
                }

                if (matched) {
                    matchedConstellations.push_back(constellation);
                }
            }

            if (matchedConstellations.empty()) {
                constellations.push_back({coordinates});
            } else {
                list<list<Coordinates>>::iterator &pivot = matchedConstellations.front();
                list<list<list<Coordinates>>::iterator>::iterator listToMergeIt = matchedConstellations.begin();

                listToMergeIt++;

                for (; listToMergeIt != matchedConstellations.end(); listToMergeIt++) {
                    pivot->splice(pivot->end(), **listToMergeIt);
                    constellations.erase(*listToMergeIt);
                }

                pivot->push_back({coordinates});
            }
        }

        fin.close();
    }

    int countConstellations() const {
        return constellations.size();
    }
};

void partOne() {
    Sky sky;

    sky.createConstellations("input");

    cout << "Solution to part one = " << sky.countConstellations() << endl;
}

int main() {
    cout << "--- Four-Dimensional Adventure ---" << endl;

    partOne();

    return 0;
}
