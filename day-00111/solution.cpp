#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int partOneRec(ifstream &fin) {
    int sum = 0, childNodes, metadataEntries, metadataEntry;

    fin >> childNodes >> metadataEntries;

    while (childNodes--) {
        sum += partOneRec(fin);
    }

    while (metadataEntries--) {
        fin >> metadataEntry;
        sum += metadataEntry;
    }

    return sum;
}

void partOne() {
    ifstream fin("input");

    cout << "Solution to part one = " << partOneRec(fin) << endl;

    fin.close();
}

int partTwoRec(ifstream &fin) {
    int sum = 0, childNodes, metadataEntries, metadataEntry;

    fin >> childNodes >> metadataEntries;

    vector<int> children;

    while (childNodes--) {
        children.push_back(partTwoRec(fin));
    }

    while (metadataEntries--) {
        fin >> metadataEntry;

        if (children.empty()) {
            sum += metadataEntry;
        } else if (metadataEntry > 0 && metadataEntry <= children.size()) {
            sum += children.at(metadataEntry - 1);
        }
    }

    return sum;
}

void partTwo() {
    ifstream fin("input");

    cout << "Solution to part two = " << partTwoRec(fin) << endl;

    fin.close();
}

int main() {
    cout << "--- Memory Maneuver ---" << endl;

    partOne();
    partTwo();

    return 0;
}
