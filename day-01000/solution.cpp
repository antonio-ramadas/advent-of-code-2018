#include <iostream>
#include <fstream>
#include <vector>
#include <list>

using namespace std;

class CircularList {
private:
    list<long long> l{0};
    list<long long>::iterator it = l.begin();

    void advance(int deltaPosition) {
        while (deltaPosition > 0) {
            deltaPosition--;
            it++;
            if (it == l.end()) {
                it = l.begin();
            }
        }

        while (deltaPosition < 0) {
            deltaPosition++;
            if (it == l.begin()) {
                it = l.end();
            }
            it--;
        }
    }

public:

    void insert(long long num, int deltaPosition) {
        advance(deltaPosition);
        it = l.insert(it, num);
    }

    long long remove(int deltaPosition) {
        advance(deltaPosition);

        long long elem = *it;

        it = l.erase(it);

        return elem;
    }

    void print() {
        for (auto el : l) {
            // This condition works, because there are no repetitive elements in the list
            if (el == *it) {
                cout << "(" << el << ") ";
            } else {
                cout << el << " ";
            }
        }

        cout << endl;
    }
};

long long partOne(const int &nPlayers, const int &nMarbles) {
    vector<long long> playerScores(nPlayers);
    CircularList cl;

    for (int i = 1; i <= nMarbles; i++) {
        if (i % 23 == 0) {
            playerScores.at((i - 1) % playerScores.size()) += i + cl.remove(-7);
        } else {
            cl.insert(i, 2);
        }
    }

    return *max_element(playerScores.begin(), playerScores.end());
}

int main() {
    cout << "--- Marble Mania ---" << endl;

    string line;

    ifstream fin("input");
    getline(fin, line);
    fin.close();

    int nPlayers, nMarbles;

    sscanf(line.c_str(), "%d players; last marble is worth %d points", &nPlayers, &nMarbles);

    cout << "Solution to part one = " << partOne(nPlayers, nMarbles) << endl;

    cout << "Solution to part two = " << partOne(nPlayers, nMarbles * 100) << endl;

    return 0;
}
