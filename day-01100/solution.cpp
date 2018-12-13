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

    bool operator<(const Cart &rhs) const {
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

    bool isVerticalConnection(int row, int col) {
        char symbol = crop.at(row).at(col);

        return symbol == '|' || symbol == '+' || symbol == '\\' || symbol == '/';
    }

    bool isHorizontalConnection(int row, int col) {
        char symbol = crop.at(row).at(col);

        return symbol == '-' || symbol == '+' || symbol == '\\' || symbol == '/';
    }

    bool shouldBeIntersection(int row, int col) {
        bool allConditionsTrue = true;

        allConditionsTrue &= row > 0;
        allConditionsTrue &= row+1 < crop.size();
        allConditionsTrue &= isVerticalConnection(row-1, col);
        allConditionsTrue &= isVerticalConnection(row+1, col);

        allConditionsTrue &= col > 0;
        allConditionsTrue &= col+1 < crop.at(row).size();
        allConditionsTrue &= isHorizontalConnection(row, col-1);
        allConditionsTrue &= isHorizontalConnection(row, col+1);

        return allConditionsTrue;
    }

    bool shouldBeHorizontalConnection(int row, int col) {
        bool allConditionsTrue = true;

        if (row > 0) {
            allConditionsTrue &= !isVerticalConnection(row-1, col);
        }

        if (row+1 < crop.size()) {
            allConditionsTrue &= !isVerticalConnection(row+1, col);
        }

        allConditionsTrue &= col > 0;
        allConditionsTrue &= col+1 < crop.at(row).size();
        allConditionsTrue &= isHorizontalConnection(row, col-1);
        allConditionsTrue &= isHorizontalConnection(row, col+1);

        return allConditionsTrue;
    }

    bool shouldBeVerticalConnection(int row, int col) {
        bool allConditionsTrue = true;

        allConditionsTrue &= row > 0;
        allConditionsTrue &= row+1 < crop.size();
        allConditionsTrue &= isVerticalConnection(row-1, col);
        allConditionsTrue &= isVerticalConnection(row+1, col);

        if (col > 0) {
            allConditionsTrue &= !isHorizontalConnection(row, col-1);
        }

        if (col+1 < crop.at(row).size()) {
            allConditionsTrue &= !isHorizontalConnection(row, col+1);
        }

        return allConditionsTrue;
    }

    bool shouldBeFrontCurve(int row, int col) {
        bool up = true, down = true;

        up &= row > 0;
        up &= col > 0;
        up &= isVerticalConnection(row-1, col) && isHorizontalConnection(row, col-1);
        if (row+1 < crop.size()) {
            up &= !isVerticalConnection(row+1, col);
        }
        if (col+1 < crop.at(row).size()) {
            up &= !isHorizontalConnection(row, col+1);
        }

        down &= row+1 < crop.size();
        down &= col+1 < crop.at(row).size();
        down &= isVerticalConnection(row+1, col) && isHorizontalConnection(row, col+1);
        if (row > 0) {
            down &= !isVerticalConnection(row-1, col);
        }
        if (col > 0) {
            up &= !isHorizontalConnection(row, col-1);
        }

        return up || down;
    }

    bool shouldBeBackCurve(int row, int col) {
        bool up = true, down = true;

        up &= row > 0;
        up &= col+1 < crop.at(row).size();
        up &= isVerticalConnection(row-1, col) && isHorizontalConnection(row, col+1);
        if (row+1 < crop.size()) {
            up &= !isVerticalConnection(row+1, col);
        }
        if (col > 0) {
            up &= !isHorizontalConnection(row, col-1);
        }

        down &= col > 0;
        down &= row+1 < crop.size();
        down &= isVerticalConnection(row+1, col) && isHorizontalConnection(row, col-1);
        if (row > 0) {
            up &= !isVerticalConnection(row-1, col);
        }
        if (col < crop.at(row).size()) {
            up &= !isHorizontalConnection(row, col+1);
        }

        return up || down;
    }
public:
    void addRow(string row) {
        int currentRowIndex = crop.size();

        for (int col = 0; col < row.size(); col++) {
            char symbol = row.at(col);

            switch (symbol) {
                case '^':
                    carts.insert(Cart(currentRowIndex, col, Direction::UP));
                    break;
                case 'v':
                    carts.insert(Cart(currentRowIndex, col, Direction::DOWN));
                    break;
                case '<':
                    carts.insert(Cart(currentRowIndex, col, Direction::LEFT));
                    break;
                case '>':
                    carts.insert(Cart(currentRowIndex, col, Direction::RIGHT));
                    break;
                default:
                    break;
            }
        }

        crop.push_back(row);
    }

    void cleanCrop() {
        for (auto cart : carts) {
            pair<int, int> coordinates = cart.getCoordinates();

            int row, col;
            row = coordinates.first;
            col = coordinates.second;

            char symbol;

            if (shouldBeIntersection(row, col)) {
                symbol = '+';
            } else if (shouldBeHorizontalConnection(row, col)) {
                symbol = '-';
            } else if (shouldBeVerticalConnection(row, col)) {
                symbol = '|';
            } else if (shouldBeFrontCurve(row, col)) {
                symbol = '/';
            } else if (shouldBeBackCurve(row, col)) {
                symbol = '\\';
            } else {
                cerr << "Missing condition at position (" << row << ", " << col << ")" << endl;
            }

            crop.at(row).at(col) = symbol;
        }
    }
};

void partOne() {
    Crop crop;
    string row;

    // Read
    ifstream fin("input");
    while (getline(fin, row)) {
        crop.addRow(row);
    }
    fin.close();

    crop.cleanCrop();

    cout << "Solution to part one = " << endl;
}

int main() {
    cout << "--- Mine Cart Madness ---" << endl;

    partOne();
    // partTwo();

    return 0;
}
