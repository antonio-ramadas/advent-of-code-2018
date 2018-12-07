#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <list>
#include <unordered_map>
#include <queue>

using namespace std;

void partOne() {

    string line;
    char node[32], child[32];
    unordered_map<string, pair<int, list<string> > > inDegree;
    priority_queue<string, vector<string>, greater<> > candidates;

    ifstream fin("input");
    while (getline(fin, line)) {
        sscanf(line.c_str(), "Step %s must be finished before step %s can begin.", node, child);

        inDegree[child].first++;
        inDegree[node].second.emplace_back(child);
    }
    fin.close();

    for (const auto &it : inDegree) {
        if (it.second.first == 0) {
            candidates.push(it.first);
        }
    }

    string solution = "", current;

    while (!inDegree.empty() && !candidates.empty()) {
        current = candidates.top();

        solution += current;

        candidates.pop();

        for (const auto &child : inDegree[current].second) {
            if (--inDegree[child].first <= 0) {
                candidates.push(child);
            }
        }

        inDegree.erase(current);
    }

    cout << "Solution to part one = " << solution << endl;
}

int main() {
    cout << "--- The Sum of Its Parts ---" << endl;

    partOne();
    // partTwo();

    return 0;
}
