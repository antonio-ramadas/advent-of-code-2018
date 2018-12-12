#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <chrono>
#include <thread>

using namespace std;

class Star {
private:
    pair<int, int> initialCoordinates, speed;
public:
    Star(int coordX, int coordY, int speedX, int speedY) {
        initialCoordinates = make_pair(coordX, coordY);
        speed = make_pair(speedX, speedY);
    }

    pair<int, int> getCoordinatesAt(int seconds) {
        int x = initialCoordinates.first + speed.first * seconds;
        int y = initialCoordinates.second + speed.second * seconds;

        return make_pair(x, y);
    }
};

class Sky {
private:
    list<Star> starCoordinates;
public:
    void addStar(Star s) {
        starCoordinates.push_back(s);
    }

    vector<vector<bool> > getSkyAt(int seconds) {
        list<pair<int, int> > stars;
        pair<int, int> coord;

        pair<int, int> height = make_pair(INT_MAX, INT_MIN);
        pair<int, int> width = make_pair(INT_MAX, INT_MIN);

        for (auto star : starCoordinates) {
            coord = star.getCoordinatesAt(seconds);

            width.first = min(width.first, coord.first);
            width.second = max(width.second, coord.first);

            height.first = min(height.first, coord.second);
            height.second = max(height.second, coord.second);

            stars.push_back(coord);
        }

        vector<vector<bool> > sky(height.second - height.first + 1, vector<bool>(width.second - width.first + 1, false));

        for (auto star : stars) {
            sky.at(star.second - height.first).at(star.first - width.first) = true;
        }

        return sky;
    }

    bool shouldPrint(const vector<vector<bool> > &newSky) {
        int slack = 20;

        bool height = newSky.size() < (starCoordinates.size() + slack);
        bool width = newSky.at(0).size() < (starCoordinates.size() + slack);

        return height && width;
    }

    void printSky(const vector<vector<bool> > &sky) {
        for (const vector<bool> &row : sky) {
            for (const bool &coord : row) {
                if (coord) {
                    cout << "#";
                } else {
                    cout << ".";
                }
            }

            cout << endl;
        }
    }
};

void partOne() {
    string line;
    int positionX, positionY, velocityX, velocityY;
    Sky sky;

    ifstream fin("input");
    while (getline(fin, line)) {
        sscanf(line.c_str(), "position=<%d,%d> velocity=<%d,%d>", &positionX, &positionY, &velocityX, &velocityY);

        sky.addStar(Star(positionX, positionY, velocityX, velocityY));
    }
    fin.close();

    vector<vector<bool> > iterSky;

    // Could also been solved using binary search assuming the solution is at the second
    //      where the distance between the most distant stars is minimum

    // The solution is at 10867
    for (int i = 10865; i < 10870; i++) {
        cout << "After " << i << " seconds:" << endl;

        iterSky = sky.getSkyAt(i);

        if (sky.shouldPrint(iterSky)) {
            sky.printSky(iterSky);
            this_thread::sleep_for(chrono::milliseconds(500));
        }

        cout << endl << endl;
    }

}

int main() {
    cout << "--- The Stars Align ---" << endl;

    cout << "Solution to part one and two = " << endl;
    partOne();

    return 0;
}
