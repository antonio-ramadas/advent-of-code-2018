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

using namespace std;

// Addition
int addr(const vector<int> &registers, const int &A, const int &B) {
    if (A < 0 || A >= registers.size() || B < 0 || B >= registers.size()) {
        throw domain_error("Bad input");
    }

    return registers.at(A) + registers.at(B);
}

int addi(const vector<int> &registers, const int &A, const int &B) {
    if (A < 0 || A >= registers.size()) {
        throw domain_error("Bad input");
    }

    return registers.at(A) + B;
}

// Multiplication
int mulr(const vector<int> &registers, const int &A, const int &B) {
    if (A < 0 || A >= registers.size() || B < 0 || B >= registers.size()) {
        throw domain_error("Bad input");
    }

    return registers.at(A) * registers.at(B);
}

int muli(const vector<int> &registers, const int &A, const int &B) {
    if (A < 0 || A >= registers.size()) {
        throw domain_error("Bad input");
    }

    return registers.at(A) * B;
}

// Bitwise AND
int banr(const vector<int> &registers, const int &A, const int &B) {
    if (A < 0 || A >= registers.size() || B < 0 || B >= registers.size()) {
        throw domain_error("Bad input");
    }

    return registers.at(A) & registers.at(B);
}

int bani(const vector<int> &registers, const int &A, const int &B) {
    if (A < 0 || A >= registers.size()) {
        throw domain_error("Bad input");
    }

    return registers.at(A) & B;
}

// Bitwise OR
int borr(const vector<int> &registers, const int &A, const int &B) {
    if (A < 0 || A >= registers.size() || B < 0 || B >= registers.size()) {
        throw domain_error("Bad input");
    }

    return registers.at(A) | registers.at(B);
}

int bori(const vector<int> &registers, const int &A, const int &B) {
    if (A < 0 || A >= registers.size()) {
        throw domain_error("Bad input");
    }

    return registers.at(A) | B;
}

// Assignment
int setr(const vector<int> &registers, const int &A, const int &B) {
    if (A < 0 || A >= registers.size()) {
        throw domain_error("Bad input");
    }

    return registers.at(A);
}

int seti(const vector<int> &registers, const int &A, const int &B) {
    return A;
}

// Greater-than testing
int gtir(const vector<int> &registers, const int &A, const int &B) {
    if (B < 0 || B >= registers.size()) {
        throw domain_error("Bad input");
    }

    return A > registers.at(B) ? 1 : 0;
}

int gtri(const vector<int> &registers, const int &A, const int &B) {
    if (A < 0 || A >= registers.size()) {
        throw domain_error("Bad input");
    }

    return registers.at(A) > B ? 1 : 0;
}

int gtrr(const vector<int> &registers, const int &A, const int &B) {
    if (A < 0 || A >= registers.size() || B < 0 || B >= registers.size()) {
        throw domain_error("Bad input");
    }

    return registers.at(A) > registers.at(B) ? 1 : 0;
}

// Equality testing
int eqir(const vector<int> &registers, const int &A, const int &B) {
    if (B < 0 || B >= registers.size()) {
        throw domain_error("Bad input");
    }

    return A == registers.at(B) ? 1 : 0;
}

int eqri(const vector<int> &registers, const int &A, const int &B) {
    if (A < 0 || A >= registers.size()) {
        throw domain_error("Bad input");
    }

    return registers.at(A) == B ? 1 : 0;
}

int eqrr(const vector<int> &registers, const int &A, const int &B) {
    if (A < 0 || A >= registers.size() || B < 0 || B >= registers.size()) {
        throw domain_error("Bad input");
    }

    return registers.at(A) == registers.at(B) ? 1 : 0;
}

// Helpers
vector<int> getArrayFromInput(const string &row) {
    vector<int> arr(4);

    int firstRegister, secondRegister, thirdRegister, fourthRegister;
    char ignore[100];

    sscanf(row.c_str(), "%s [%d, %d, %d, %d]", ignore, &firstRegister, &secondRegister, &thirdRegister, &fourthRegister);

    arr.at(0) = firstRegister;
    arr.at(1) = secondRegister;
    arr.at(2) = thirdRegister;
    arr.at(3) = fourthRegister;

    return arr;
}

void partOne() {
    vector<int> before, after;
    int opcode, A, B, C;
    unsigned int nAmbiguous = 0, count;
    string row;

    vector<function<int(vector<int>, int, int)> > operations;

    operations = {
            addr, addi,
            mulr, muli,
            banr, bani,
            borr, bori,
            setr, seti,
            gtir, gtri, gtrr,
            eqir, eqri, eqrr
    };

    ifstream fin("input");

    while (getline(fin, row) && !row.empty()) {
        before = getArrayFromInput(row);

        fin >> opcode >> A >> B >> C;

        fin.ignore(1000, '\n');
        getline(fin, row);
        after = getArrayFromInput(row);

        fin.ignore(1000, '\n');

        count = 0;

        for (unsigned long i = 0; i < operations.size() && count < 3; i++) {
            try {
                if (after.at(C) == operations.at(i)(before, A, B)) {
                    count++;
                }
            } catch (domain_error &err) {
                // Just ignore and move to the next one
            }
        }

        if (count >= 3) {
            nAmbiguous++;
        }
    }

    fin.close();

    cout << "Solution to part one = " << nAmbiguous << endl;
}

void partTwo() {
    vector<int> before, after;
    int opcode, A, B, C;
    bool validOperation;
    vector<function<int(vector<int>, int, int)> > possibleOperations;
    possibleOperations = {
            addr, addi,
            mulr, muli,
            banr, bani,
            borr, bori,
            setr, seti,
            gtir, gtri, gtrr,
            eqir, eqri, eqrr
    };
    list<int> operationIndices(16);
    iota(operationIndices.begin(), operationIndices.end(), 0);
    vector<set<int> > operations(16, set<int>(operationIndices.begin(), operationIndices.end()));
    string row;

    ifstream fin("input");

    while (getline(fin, row) && !row.empty()) {
        before = getArrayFromInput(row);

        fin >> opcode >> A >> B >> C;

        fin.ignore(1000, '\n');
        getline(fin, row);
        after = getArrayFromInput(row);

        fin.ignore(1000, '\n');

        auto &listOfOperationsIndices = operations.at(opcode);
        auto it = listOfOperationsIndices.begin();

        while (it != listOfOperationsIndices.end()) {
            try {
                validOperation = after.at(C) == possibleOperations.at(*it)(before, A, B);
            } catch (domain_error &err) {
                validOperation = false;
            }

            if (validOperation) {
                it++;
            } else {
                it = listOfOperationsIndices.erase(it);
            }
        }

        // If we are sure about a certain opcode, then we can remove it from the rest
        if (listOfOperationsIndices.size() == 1) {
            for (unsigned long i = 0; i < operations.size(); i++) {
                if (i != opcode) {
                    operations.at(i).erase(*listOfOperationsIndices.begin());
                }
            }
        }
    }

    fin.ignore(1000, '\n');

    after = {0, 0, 0, 0};

    while (fin >> opcode >> A >> B >> C) {
        after.at(C) = possibleOperations.at(*operations.at(opcode).begin())(after, A, B);
    }

    fin.close();

    cout << "Solution to part two = " << after.at(0) << endl;
}

int main() {
    cout << "--- Chronal Classification ---" << endl;

    partOne();
    partTwo();

    return 0;
}