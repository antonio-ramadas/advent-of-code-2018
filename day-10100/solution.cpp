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

    for (; registers.at(ipIndex) >= 0 && registers.at(ipIndex) < instructions.size(); registers.at(ipIndex)++) {
        instructions.at(registers.at(ipIndex))(registers);
    }

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
