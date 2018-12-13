#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

enum class Direction { LEFT, RIGHT, UP, DOWN };
enum class Turn { LEFT, STRAIGHT, RIGHT };

class Cart {
private:
    pair<int, int> coordinates;
    Direction direction;
    Turn turn = Turn::LEFT;

public:
    Cart(int row, int col, Direction dir) {
        coordinates = make_pair(row, col);
        direction = dir;
    }

    bool operator<(const Cart &rhs) {
        return getCoordinates() < rhs.getCoordinates();
    }

    pair<int, int> getCoordinates() const {
        return coordinates;
    }
};

class Crop {
private:
    vector<string> crop;
    set<Cart> carts;
};

void partOne() {
    // Read
    ifstream fin("input");
    fin.close();

    cout << "Solution to part one = " << endl;
}

int main() {
    cout << "--- Mine Cart Madness ---" << endl;

    partOne();
    // partTwo();

    return 0;
}
