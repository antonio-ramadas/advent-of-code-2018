#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>

using namespace std;

typedef pair<int, int> coordinates;

coordinates operator+(const coordinates &lhs, const coordinates &rhs) {
    return make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

coordinates operator-(const coordinates &lhs, const coordinates &rhs) {
    return make_pair(lhs.first - rhs.first, lhs.second - rhs.second);
}

coordinates& operator+=(coordinates &lhs, const coordinates &rhs) {
    lhs.first += rhs.first;
    lhs.second += rhs.second;
    return lhs;
}

class Character {
    static unsigned long staticId;
    unsigned long id;
    int health = 200;
    unsigned int damageHit = 3;
    char symbol, enemySymbol;
public:
    explicit Character(const char &sym, unsigned int damageHit = 3) {
        this->damageHit = damageHit;
        symbol = sym;
        enemySymbol = symbol == 'E' ? 'G' : 'E';
        id = staticId++;
    }

    bool isElf() const {
        return symbol == 'E';
    }

    bool isDead() {
        return health <= 0;
    }

    bool hit() {
        health -= damageHit;
        return isDead();
    }

    int getHealth() const {
        return health;
    }

    bool operator==(const Character &anotherCharacter) const {
        return id == anotherCharacter.id;
    }
};

unsigned long Character::staticId = 0;

class Grid {
private:
    vector<string> grid;
    map<coordinates, Character> elves, goblins;
    unsigned long time = 0;
    list<pair<int, int> > readingOrder;
    unsigned int goblinsDamageHit = 3;
    bool hasAtLeastAnElfDied = false;

    map<coordinates, Character> getAllCharacters() {
        map<coordinates, Character> characters = elves;

        characters.insert(goblins.begin(), goblins.end());

        return characters;
    }

    set<coordinates> getAllCharactersCoordinates() {
        set<coordinates> charactersCoordinates;

        for (const auto &kv : elves) {
            charactersCoordinates.insert(kv.first);
        }

        for (const auto &kv : goblins) {
            charactersCoordinates.insert(kv.first);
        }

        return charactersCoordinates;
    }

    bool isValidEmptyPosition(const coordinates &position) {
        bool isInBounds = position.first >= 0 &&
                          position.second >= 0 &&
                          position.first < grid.size() &&
                          position.second < grid.at(0).size();

        if (!isInBounds) {
            return false;
        }

        set<coordinates> charactersCoordinates = getAllCharactersCoordinates();

        bool isAnyCharacterAtPosition = charactersCoordinates.find(position) != charactersCoordinates.end();

        bool isValidPositionToBe = grid.at(position.first).at(position.second) == '.';

        return isInBounds && !isAnyCharacterAtPosition && isValidPositionToBe;
    }

    bool isCharacterAt(const coordinates &position) {
        bool isInBounds = position.first >= 0 &&
                          position.second >= 0 &&
                          position.first < grid.size() &&
                          position.second < grid.at(0).size();

        if (!isInBounds) {
            return false;
        }

        set<coordinates> charactersCoordinates = getAllCharactersCoordinates();

        bool isAnyCharacterAtPosition = charactersCoordinates.find(position) != charactersCoordinates.end();

        return isInBounds && isAnyCharacterAtPosition;
    }

    unsigned int getDistanceBetweenCoordinates(const coordinates &lhs, const coordinates &rhs) {
        return abs(lhs.first - rhs.first) + abs(lhs.second - rhs.second);
    }

    vector<vector<unsigned int> > buildDistanceMatrix(const coordinates &start) {
        vector<vector<unsigned int> > matrix(grid.size(), vector<unsigned int>(grid.at(0).size(), INT_MAX));
        queue<coordinates> q;

        if (!isValidEmptyPosition(start)) {
            return matrix;
        }

        matrix.at(start.first).at(start.second) = 0;
        q.push(start);

        coordinates currentCoordinates;
        unsigned int currentDistance;

        while (!q.empty()) {
            for (const coordinates &direction : readingOrder) {
                currentCoordinates = q.front() + direction;

                if (isValidEmptyPosition(currentCoordinates) || isCharacterAt(currentCoordinates)) {
                    currentDistance = matrix.at(q.front().first).at(q.front().second);

                    if (currentDistance + 1 < matrix.at(currentCoordinates.first).at(currentCoordinates.second)) {
                        matrix.at(currentCoordinates.first).at(currentCoordinates.second) = currentDistance + 1;
                        if (isValidEmptyPosition(currentCoordinates)) {
                            q.push(currentCoordinates);
                        }
                    }
                }
            }

            q.pop();
        }

        return matrix;
    }

    unsigned int getMinimumDistanceToEnemy(const coordinates &start, const map<coordinates, Character> &enemies) {
        vector<vector<unsigned int> > matrix(grid.size(), vector<unsigned int>(grid.at(0).size(), INT_MAX));
        queue<coordinates> q;

        if (!isValidEmptyPosition(start)) {
            return INT_MAX;
        }

        matrix.at(start.first).at(start.second) = 0;
        q.push(start);

        coordinates currentCoordinates;
        unsigned int currentDistance;

        while (!q.empty()) {
            for (const coordinates &direction : readingOrder) {
                currentCoordinates = q.front() + direction;

                if (enemies.find(currentCoordinates) != enemies.end()) {
                    return matrix.at(q.front().first).at(q.front().second) + 1;
                }

                if (isValidEmptyPosition(currentCoordinates) || isCharacterAt(currentCoordinates)) {
                    currentDistance = matrix.at(q.front().first).at(q.front().second);

                    if (currentDistance + 1 < matrix.at(currentCoordinates.first).at(currentCoordinates.second)) {
                        matrix.at(currentCoordinates.first).at(currentCoordinates.second) = currentDistance + 1;
                        if (isValidEmptyPosition(currentCoordinates)) {
                            q.push(currentCoordinates);
                        }
                    }
                }
            }

            q.pop();
        }

        return INT_MAX;
    }

    // Dijkstra (BFS) (Stops on the first enemy found)
    coordinates getBestMoveDirectionImproved(const coordinates &characterCoordinates, const map<coordinates, Character> &enemies) {
        // The default best move is to stay in the same place
        coordinates bestMove = make_pair(0, 0);

        for (const auto &enemy : enemies) {
            // If the character is right next to an enemy...
            if (getDistanceBetweenCoordinates(characterCoordinates, enemy.first) <= 1) {
                return bestMove;
            }
        }

        // Get closest distance to an enemy
        coordinates enemyCoordinates;
        vector<vector<unsigned int> > distancesMatrix;
        unsigned int minimumDistance = INT_MAX, distanceToEnemy;
        for (const coordinates &direction : readingOrder) {

            distanceToEnemy = getMinimumDistanceToEnemy(characterCoordinates + direction, enemies);

            if (distanceToEnemy < minimumDistance) {
                minimumDistance = distanceToEnemy;
                bestMove = direction;
            }
        }

        return bestMove;
    }

    // Dijkstra (BFS)
    coordinates getBestMoveDirection(const coordinates &characterCoordinates, const map<coordinates, Character> &enemies) {
        // The default best move is to stay in the same place
        coordinates bestMove = make_pair(0, 0);

        for (const auto &enemy : enemies) {
            // If the character is right next to an enemy...
            if (getDistanceBetweenCoordinates(characterCoordinates, enemy.first) <= 1) {
                return bestMove;
            }
        }

        // Get closest distance to an enemy
        coordinates enemyCoordinates;
        vector<vector<unsigned int> > distancesMatrix;
        unsigned int minimumDistance = INT_MAX, distanceToEnemy;
        for (const coordinates &direction : readingOrder) {

            distancesMatrix = buildDistanceMatrix(characterCoordinates + direction);

            for (const auto &enemy : enemies) {
                enemyCoordinates = enemy.first;

                distanceToEnemy = distancesMatrix.at(enemyCoordinates.first).at(enemyCoordinates.second);

                if (distanceToEnemy < minimumDistance) {
                    minimumDistance = distanceToEnemy;
                    bestMove = direction;
                }
            }
        }

        return bestMove;
    }

    void attack(coordinates characterCoordinates, map<coordinates, Character> &enemies) {
        bool enemyFound = false;
        coordinates enemyCoordinates;

        for (const auto &enemy : enemies) {
            // If the character is right next to an enemy...
            if (getDistanceBetweenCoordinates(characterCoordinates, enemy.first) <= 1) {
                if (!enemyFound || enemy.second.getHealth() < enemies.at(enemyCoordinates).getHealth()) {
                    enemyFound = true;
                    enemyCoordinates = enemy.first;
                }
            }
        }

        if (enemyFound) {
            enemies.at(enemyCoordinates).hit();
            if (enemies.at(enemyCoordinates).isDead()) {
                if (enemies.at(enemyCoordinates).isElf()) {
                    hasAtLeastAnElfDied = true;
                }
                enemies.erase(enemyCoordinates);
            }
        }
    }

    void act(coordinates characterCoordinates, map<coordinates, Character> &friends, map<coordinates, Character> &enemies) {
        coordinates moveDirection = getBestMoveDirectionImproved(characterCoordinates, enemies);

        // Move
        Character currentCharacter = friends.at(characterCoordinates);
        friends.erase(characterCoordinates);
        characterCoordinates += moveDirection;
        friends.emplace(characterCoordinates, currentCharacter);

        attack(characterCoordinates, enemies);
    }

public:
    Grid(unsigned int goblinsDamageHit = 3) {
        this->goblinsDamageHit = goblinsDamageHit;

        // top -> left -> right -> bottom
        readingOrder.emplace_back(-1, 0);
        readingOrder.emplace_back(0, -1);
        readingOrder.emplace_back(0, 1);
        readingOrder.emplace_back(1, 0);
    }

    void addRow(string row) {
        unsigned long currentRowIndex = grid.size();

        for (unsigned long colIndex = 0; colIndex < row.size(); colIndex++) {
            switch (row.at(colIndex)) {
                case 'G':
                    goblins.emplace(make_pair(currentRowIndex, colIndex), Character('G', goblinsDamageHit));
                    row.at(colIndex) = '.';
                    break;
                case 'E':
                    elves.emplace(make_pair(currentRowIndex, colIndex), Character('E'));
                    row.at(colIndex) = '.';
                    break;
                default:
                    continue;
            }
        }

        grid.push_back(row);
    }

    void print() {
        cout << "Current tick: " << time << endl;

        char symbolToPrint;

        for (unsigned long row = 0; row < grid.size(); row++) {
            for (unsigned long col = 0; col < grid.at(row).size(); col++) {
                if (elves.find(make_pair(row, col)) != elves.end()) {
                    symbolToPrint = 'E';
                } else if (goblins.find(make_pair(row, col)) != goblins.end()) {
                    symbolToPrint = 'G';
                } else {
                    symbolToPrint = grid.at(row).at(col);
                }

                cout << symbolToPrint;
            }
            cout << endl;
        }

        cout << endl;
    }

    void tick() {
        for (auto &character : getAllCharacters()) {
            if (elves.find(character.first) != elves.end() && elves.at(character.first) == character.second) {
                act(character.first, elves, goblins);
            } else if (goblins.find(character.first) != goblins.end() && goblins.at(character.first) == character.second) {
                act(character.first, goblins, elves);
            }
            // else { character died in this tick }
        }

        if (!isItOver()) {
            time++;
        }
    }

    bool isItOver() {
        return elves.empty() || goblins.empty();
    }

    unsigned long getScore() {
        unsigned long score = 0;

        for (const auto &character : elves) {
            score += character.second.getHealth();
        }

        for (const auto &character : goblins) {
            score += character.second.getHealth();
        }

        return score * time;
    }

    bool hasOneOrMoreElvesDied() {
        return hasAtLeastAnElfDied;
    }

    void setHasAtLeastAnElfDied(const bool &state) {
        hasAtLeastAnElfDied = state;
    }
};

void partOne() {
    Grid g;
    string row;

    // Read
    ifstream fin("input");
    while (getline(fin, row)) {
        g.addRow(row);
    }
    fin.close();

    // The code itself is not very efficient
    // There are two ways to improve the speed:
    //   1. Refactor the code (tradeoff between easy-to-understand and efficency)
    //   2. Cache common steps

    g.print();

    // Play
    while (!g.isItOver()) {
        g.tick();
        g.print();
    }

    cout << "Solution to part one = " << g.getScore() << endl;
}

void partTwo() {
    Grid g;

    g.setHasAtLeastAnElfDied(true);

    // A binary search would be faster...
    for (unsigned int goblinsDamageHit = 4; g.hasOneOrMoreElvesDied(); goblinsDamageHit++) {
        g = Grid(goblinsDamageHit);
        string row;

        // Read
        ifstream fin("input");
        while (getline(fin, row)) {
            g.addRow(row);
        }
        fin.close();

        cout << "Goblins have now a " << goblinsDamageHit << " damage hit" << endl;

        g.print();

        // Play
        while (!g.isItOver() && !g.hasOneOrMoreElvesDied()) {
            g.tick();
            g.print();
        }
    }

    // Damage is 20
    cout << "Solution to part two = " << g.getScore() << endl;
}

int main() {
    cout << "--- Beverage Bandits ---" << endl;

    partOne();
    partTwo();

    return 0;
}