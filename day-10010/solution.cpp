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
        instructions.push_back(bind(functions[operation], _1, A, B, C));
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

    for (; registers.at(ipIndex) >= 0 && registers.at(ipIndex) < instructions.size(); registers.at(ipIndex)++) {
        instructions.at(registers.at(ipIndex))(registers);
    }

    cout << "Solution to part one = " << registers.at(0) << endl;
}

void partTwo() {
    vector<int> registers(6, 0);
    registers.at(0) = 1;

    // No need to read from input, as it was fully reverse engineering assuming that the first registers equals to 1

    // [17, 20]
    registers.at(4) =  (registers.at(4) + 2) * (registers.at(4) + 2) * 19 * 11;
    // [21, 23]
    registers.at(5) = (registers.at(5) + 1) * 22 + 17;

    // 24
    registers.at(4) += registers.at(5);

    // [25, 32] (skips 26, because it is assumed registers.at(0) == 1)
    registers.at(5) = (27 * 28 + 29) * 30 * 14 * 32;

    // 33
    registers.at(4) += registers.at(5);

    // 34
    registers.at(0) = 0;
    // 1
    registers.at(1) = 1;

    // If you reverse the "assembly" instruction to C++, it should look something like this
    /*do {
        // 2
        registers.at(3) = 1;

        do {
            // 3
            registers.at(5) = registers.at(1) * registers.at(3);

            // 4
            registers.at(5) = registers.at(5) == registers.at(4) ? 1 : 0;

            if (registers.at(5) == 1) {
                // 7
                registers.at(0) += registers.at(1);
            }

            // 8
            registers.at(3) += 1;

            // 9
            registers.at(5) = registers.at(3) > registers.at(4) ? 1 : 0;

        } while (registers.at(5) != 1);

        // 12
        registers.at(1) += 1;

        // 13
        registers.at(5) = registers.at(1) > registers.at(4) ? 1 : 0;

    } while (registers.at(5) != 1);*/

    // This loop does the same as the code commented above
    for (; registers.at(1) <= registers.at(4); registers.at(1)++) {
        if (registers.at(4) % registers.at(1) == 0) {
            registers.at(0) += registers.at(4) / registers.at(1);
        }
    }

    cout << "Solution to part two = " << registers.at(0) << endl;
}

int main() {
    cout << "--- Go With The Flow ---" << endl;

    partOne();
    partTwo();

    return 0;
}