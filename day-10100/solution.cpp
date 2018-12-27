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
#include <iomanip>

using namespace std;
using namespace std::placeholders;

typedef __bind<function<void(vector<int> &, const int &, const int &,
                             const int &)> &, const __ph<1> &, int &, int &, int &> operationBind;

// Addition
void addr(vector<int> &registers, const int &A, const int &B, const int &C) {
    if (A < 0 || A >= registers.size() || B < 0 || B >= registers.size()) {
        throw domain_error("Bad input");
    }

    registers.at(C) = registers.at(A) + registers.at(B);
}

void addi(vector<int> &registers, const int &A, const int &B, const int &C) {
    if (A < 0 || A >= registers.size()) {
        throw domain_error("Bad input");
    }

    registers.at(C) = registers.at(A) + B;
}

// Multiplication
void mulr(vector<int> &registers, const int &A, const int &B, const int &C) {
    if (A < 0 || A >= registers.size() || B < 0 || B >= registers.size()) {
        throw domain_error("Bad input");
    }

    registers.at(C) = registers.at(A) * registers.at(B);
}

void muli(vector<int> &registers, const int &A, const int &B, const int &C) {
    if (A < 0 || A >= registers.size()) {
        throw domain_error("Bad input");
    }

    registers.at(C) = registers.at(A) * B;
}

// Bitwise AND
void banr(vector<int> &registers, const int &A, const int &B, const int &C) {
    if (A < 0 || A >= registers.size() || B < 0 || B >= registers.size()) {
        throw domain_error("Bad input");
    }

    registers.at(C) = registers.at(A) & registers.at(B);
}

void bani(vector<int> &registers, const int &A, const int &B, const int &C) {
    if (A < 0 || A >= registers.size()) {
        throw domain_error("Bad input");
    }

    registers.at(C) = registers.at(A) & B;
}

// Bitwise OR
void borr(vector<int> &registers, const int &A, const int &B, const int &C) {
    if (A < 0 || A >= registers.size() || B < 0 || B >= registers.size()) {
        throw domain_error("Bad input");
    }

    registers.at(C) = registers.at(A) | registers.at(B);
}

void bori(vector<int> &registers, const int &A, const int &B, const int &C) {
    if (A < 0 || A >= registers.size()) {
        throw domain_error("Bad input");
    }

    registers.at(C) = registers.at(A) | B;
}

// Assignment
void setr(vector<int> &registers, const int &A, const int &B, const int &C) {
    if (A < 0 || A >= registers.size()) {
        throw domain_error("Bad input");
    }

    registers.at(C) = registers.at(A);
}

void seti(vector<int> &registers, const int &A, const int &B, const int &C) {
    registers.at(C) = A;
}

// Greater-than testing
void gtir(vector<int> &registers, const int &A, const int &B, const int &C) {
    if (B < 0 || B >= registers.size()) {
        throw domain_error("Bad input");
    }

    registers.at(C) = A > registers.at(B) ? 1 : 0;
}

void gtri(vector<int> &registers, const int &A, const int &B, const int &C) {
    if (A < 0 || A >= registers.size()) {
        throw domain_error("Bad input");
    }

    registers.at(C) = registers.at(A) > B ? 1 : 0;
}

void gtrr(vector<int> &registers, const int &A, const int &B, const int &C) {
    if (A < 0 || A >= registers.size() || B < 0 || B >= registers.size()) {
        throw domain_error("Bad input");
    }

    registers.at(C) = registers.at(A) > registers.at(B) ? 1 : 0;
}

// Equality testing
void eqir(vector<int> &registers, const int &A, const int &B, const int &C) {
    if (B < 0 || B >= registers.size()) {
        throw domain_error("Bad input");
    }

    registers.at(C) = A == registers.at(B) ? 1 : 0;
}

void eqri(vector<int> &registers, const int &A, const int &B, const int &C) {
    if (A < 0 || A >= registers.size()) {
        throw domain_error("Bad input");
    }

    registers.at(C) = registers.at(A) == B ? 1 : 0;
}

void eqrr(vector<int> &registers, const int &A, const int &B, const int &C) {
    if (A < 0 || A >= registers.size() || B < 0 || B >= registers.size()) {
        throw domain_error("Bad input");
    }

    registers.at(C) = registers.at(A) == registers.at(B) ? 1 : 0;
}

vector<operationBind> getInstructions(ifstream &fin) {
    vector<operationBind> instructions;

    unordered_map<string, function<void(vector<int> &, const int &, const int &, const int &)>> functions;

    functions["addr"] = addr;
    functions["addi"] = addi;

    functions["mulr"] = mulr;
    functions["muli"] = muli;

    functions["banr"] = banr;
    functions["bani"] = bani;
    functions["bani"] = bani;

    functions["borr"] = borr;
    functions["bori"] = bori;

    functions["setr"] = setr;
    functions["seti"] = seti;

    functions["gtir"] = gtir;
    functions["gtri"] = gtri;
    functions["gtrr"] = gtrr;

    functions["eqir"] = eqir;
    functions["eqri"] = eqri;
    functions["eqrr"] = eqrr;

    string operation;
    int A, B, C;

    while (fin >> operation >> A >> B >> C) {
        instructions.push_back(bind(functions[operation], placeholders::_1, A, B, C));
    }

    return instructions;
}

void partOne() {
    vector<int> registers(6, 0);
    int ipIndex;
    vector<operationBind> instructions;

    ifstream fin("input");

    fin.ignore(1000, ' ');
    fin >> ipIndex;
    instructions = getInstructions(fin);
    fin.close();

    int lowestInteger = 21;
    registers.at(0) = lowestInteger;

    /*for (; registers.at(ipIndex) >= 0 && registers.at(ipIndex) < instructions.size(); registers.at(ipIndex)++) {
        instructions.at(registers.at(ipIndex))(registers);
    }*/


    ////////////////////////

    bool loop = true;

    // [0, 2]
    registers.at(3) = ((123 & 456) == 72) ? 1 : 0;

    // Skips 4, because registers.at(3) == 1

    // 5
    registers.at(3) = 0;

    do {
        // 6
        registers.at(1) = registers.at(3) | 65536;

        // 7
        registers.at(3) = 10373714;

        // 8
        registers.at(5) = registers.at(1) & 255;

        // 9
        registers.at(3) += registers.at(5);

        // 10
        registers.at(3) &= 16777215;

        // 11
        registers.at(3) *= 65899;

        // 12
        registers.at(3) &= 16777215;

        // 13
        registers.at(5) = (256 > registers.at(1)) ? 1 : 0;

        // [14, 15]
        if (registers.at(5) == 1) {
            // 28
            registers.at(5) = (registers.at(3) == registers.at(0)) ? 1 : 0;
        }

        loop = registers.at(5) == 0;

    } while (loop);

    // Finished

    // 17
    registers.at(5) = 0;

    do {
        // [18, 19]
        registers.at(4) = (registers.at(5) + 1) * 256;

        // 20
        registers.at(4) = (registers.at(4) > registers.at(1)) ? 1 : 0;

        // [21, 22]
        if (registers.at(4) == 1) {
            // 26
            registers.at(1) = registers.at(5);

            // go to line 8
        } else {
            // 24
            registers.at(5) += 1;
        }

        // 25
    } while (true);


    cout << "Solution to part one = " << lowestInteger << endl;
}

void partTwo() {
    cout << "Solution to part two = " << endl;
}

int main() {
    cout << "--- Chronal Conversion ---" << endl;

    partOne();
    partTwo();

    return 0;
}
