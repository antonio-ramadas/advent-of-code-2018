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

    void updateTurn() {
        switch (turn) {
            case Turn::LEFT:
                turn = Turn::STRAIGHT;
                break;
            case Turn::STRAIGHT:
                turn = Turn::RIGHT;
                break;
            case Turn::RIGHT:
                turn = Turn::LEFT;
                break;
            default:
                cerr << "Wrong turn!" << endl;
                break;
        }
    }

    void updateIntersectionDirection() {
        switch (turn) {
            case Turn::LEFT:
                switch (direction) {
                    case Direction::UP:
                        direction = Direction::LEFT;
                        break;
                    case Direction::DOWN:
                        direction = Direction::RIGHT;
                        break;
                    case Direction::LEFT:
                        direction = Direction::DOWN;
                        break;
                    case Direction::RIGHT:
                        direction = Direction::UP;
                        break;
                }
                break;
            case Turn::STRAIGHT:
                break;
            case Turn::RIGHT:
                switch (direction) {
                    case Direction::UP:
                        direction = Direction::RIGHT;
                        break;
                    case Direction::DOWN:
                        direction = Direction::LEFT;
                        break;
                    case Direction::LEFT:
                        direction = Direction::UP;
                        break;
                    case Direction::RIGHT:
                        direction = Direction::DOWN;
                        break;
                }
                break;
            default:
                cerr << "Wrong turn!" << endl;
                break;
        }

        updateTurn();
    }

    void updateFrontCurveDirection() {
        switch (direction) {
            case Direction::UP:
                direction = Direction::RIGHT;
                break;
            case Direction::DOWN:
                direction = Direction::LEFT;
                break;
            case Direction::LEFT:
                direction = Direction::DOWN;
                break;
            case Direction::RIGHT:
                direction = Direction::UP;
                break;
        }
    }

    void updateBackDirection() {
        switch (direction) {
            case Direction::UP:
                direction = Direction::LEFT;
                break;
            case Direction::DOWN:
                direction = Direction::RIGHT;
                break;
            case Direction::LEFT:
                direction = Direction::UP;
                break;
            case Direction::RIGHT:
                direction = Direction::DOWN;
                break;
        }
    }

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

    Direction getDirection() const {
        return direction;
    }

    Turn getTurn() const {
        return turn;
    }

    void updateDirection(const char &symbol) {
        switch (symbol) {
            case '+':
                updateIntersectionDirection();
                break;
            case '/':
                updateFrontCurveDirection();
                break;
            case '\\':
                updateBackDirection();
                break;
            default:
                break;
        }
    }

    void move() {
        pair<int, int> dir;

        switch (direction) {
            case Direction::UP:
                dir = make_pair(-1, 0);
                break;
            case Direction::DOWN:
                dir = make_pair(+1, 0);
                break;
            case Direction::LEFT:
                dir = make_pair(0, -1);
                break;
            case Direction::RIGHT:
                dir = make_pair(0, +1);
                break;
        }

        coordinates = make_pair(coordinates.first + dir.first, coordinates.second + dir.second);
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

        allConditionsTrue = allConditionsTrue && row > 0;
        allConditionsTrue = allConditionsTrue && row+1 < crop.size();
        allConditionsTrue = allConditionsTrue && isVerticalConnection(row-1, col);
        allConditionsTrue = allConditionsTrue && isVerticalConnection(row+1, col);

        allConditionsTrue = allConditionsTrue && col > 0;
        allConditionsTrue = allConditionsTrue && col+1 < crop.at(row).size();
        allConditionsTrue = allConditionsTrue && isHorizontalConnection(row, col-1);
        allConditionsTrue = allConditionsTrue && isHorizontalConnection(row, col+1);

        return allConditionsTrue;
    }

    bool shouldBeHorizontalConnection(int row, int col) {
        bool allConditionsTrue = true;

        if (row > 0) {
            allConditionsTrue = allConditionsTrue && !isVerticalConnection(row-1, col);
        }

        if (row+1 < crop.size()) {
            allConditionsTrue = allConditionsTrue && !isVerticalConnection(row+1, col);
        }

        allConditionsTrue = allConditionsTrue && col > 0;
        allConditionsTrue = allConditionsTrue && col+1 < crop.at(row).size();
        allConditionsTrue = allConditionsTrue && isHorizontalConnection(row, col-1);
        allConditionsTrue = allConditionsTrue && isHorizontalConnection(row, col+1);

        return allConditionsTrue;
    }

    bool shouldBeVerticalConnection(int row, int col) {
        bool allConditionsTrue = true;

        allConditionsTrue = allConditionsTrue && row > 0;
        allConditionsTrue = allConditionsTrue && row+1 < crop.size();
        allConditionsTrue = allConditionsTrue && isVerticalConnection(row-1, col);
        allConditionsTrue = allConditionsTrue && isVerticalConnection(row+1, col);

        if (col > 0) {
            allConditionsTrue = allConditionsTrue && !isHorizontalConnection(row, col-1);
        }

        if (col+1 < crop.at(row).size()) {
            allConditionsTrue = allConditionsTrue && !isHorizontalConnection(row, col+1);
        }

        return allConditionsTrue;
    }

    bool shouldBeFrontCurve(int row, int col) {
        bool up = true, down = true;

        up = up && row > 0;
        up = up && col > 0;
        up = up && isVerticalConnection(row-1, col) && isHorizontalConnection(row, col-1);
        if (row+1 < crop.size()) {
            up = up && !isVerticalConnection(row+1, col);
        }
        if (col+1 < crop.at(row).size()) {
            up = up && !isHorizontalConnection(row, col+1);
        }

        down = down && row+1 < crop.size();
        down = down && col+1 < crop.at(row).size();
        down = down && isVerticalConnection(row+1, col) && isHorizontalConnection(row, col+1);
        if (row > 0) {
            down = down && !isVerticalConnection(row-1, col);
        }
        if (col > 0) {
            down = down && !isHorizontalConnection(row, col-1);
        }

        return up || down;
    }

    bool shouldBeBackCurve(int row, int col) {
        bool up = true, down = true;

        up = up && row > 0;
        up = up && col+1 < crop.at(row).size();
        up = up && isVerticalConnection(row-1, col) && isHorizontalConnection(row, col+1);
        if (row+1 < crop.size()) {
            up = up && !isVerticalConnection(row+1, col);
        }
        if (col > 0) {
            up = up && !isHorizontalConnection(row, col-1);
        }

        down = down && col > 0;
        down = down && row+1 < crop.size();
        down = down && isVerticalConnection(row+1, col) && isHorizontalConnection(row, col-1);
        if (row > 0) {
            down = down && !isVerticalConnection(row-1, col);
        }
        if (col < crop.at(row).size()) {
            down = down && !isHorizontalConnection(row, col+1);
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

    bool tick(pair<int, int> &crashCoordinates) {
        set<Cart> tmpCarts = carts;
        set<Cart> newCarts;

        for (const Cart &cart : carts) {
            Cart newCart = cart;
            pair<int, int> cartCoordinates = newCart.getCoordinates();

            newCart.updateDirection(crop.at(cartCoordinates.first).at(cartCoordinates.second));

            newCart.move();

            tmpCarts.erase(cart);

            // If it crashes with any of the previous ones or the ones that already moved...
            if (tmpCarts.find(newCart) != tmpCarts.end() || newCarts.find(newCart) != newCarts.end()) {
                crashCoordinates = newCart.getCoordinates();
                return false;
            }

            newCarts.insert(newCart);
        }

        carts = newCarts;
        return true;
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

    pair<int, int> crashCoordinates;

    while (crop.tick(crashCoordinates));

    cout << "Solution to part one = " << crashCoordinates.second << "," << crashCoordinates.first << endl;
}

int main() {
    cout << "--- Mine Cart Madness ---" << endl;

    partOne();
    // partTwo();

    return 0;
}
