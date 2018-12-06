#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <list>

using namespace std;

void partOne() {
    int x, y, maxX = -1, maxY = -1;
    char ignore;
    list<pair<int, int> > coordinates;

    ifstream fin("input");
    while (fin >> x >> ignore >> y) {
        coordinates.emplace_back(x, y);
        maxX = max(maxX, x);
        maxY = max(maxY, y);
    }
    fin.close();

    vector<vector<pair<int, int> > > grid(maxY, vector<pair<int, int> >(maxX, make_pair(-1, -1)));



    cout << "Solution to part one = " << endl;
}

int main() {
    cout << "--- Chronal Coordinates ---" << endl;

    partOne();

    return 0;
}
