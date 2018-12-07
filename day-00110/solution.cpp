#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <list>
#include <unordered_map>
#include <queue>
#include <set>

using namespace std;

void partOne() {

    string line;
    unordered_map<char, pair<int, list<char> > > inDegree;
    priority_queue<char, vector<char>, greater<> > candidates;

    ifstream fin("input");
    while (getline(fin, line)) {
        char node, child;
        sscanf(line.c_str(), "Step %c must be finished before step %c can begin.", &node, &child);

        inDegree[child].first++;
        inDegree[node].second.emplace_back(child);
    }
    fin.close();

    for (const auto &it : inDegree) {
        if (it.second.first == 0) {
            candidates.push(it.first);
        }
    }

    string solution;
    char current;

    while (!inDegree.empty() && !candidates.empty()) {
        current = candidates.top();

        solution += current;

        candidates.pop();

        for (const auto &child : inDegree[current].second) {
            if (--inDegree[child].first == 0) {
                candidates.push(child);
            }
        }

        inDegree.erase(current);
    }

    cout << "Solution to part one = " << solution << endl;
}

void partTwo() {
    string line;
    unordered_map<char, pair<int, list<char> > > inDegree;
    set<pair<int, char> > readyToWorkOn;
    set<pair<int, int> > active;

    ifstream fin("input");
    while (getline(fin, line)) {
        char node, child;

        sscanf(line.c_str(), "Step %c must be finished before step %c can begin.", &node, &child);

        inDegree[child].first++;
        inDegree[node].second.emplace_back(child);
    }
    fin.close();

    // Get first jobs
    for (const auto &it : inDegree) {
        if (it.second.first == 0) {
            readyToWorkOn.emplace(it.first - 'A' + 1 + 60, it.first);
        }
    }

    int solution = 0;

    while (!readyToWorkOn.empty() || !active.empty()) {
        // Add work that is ready to the active workers
        while (active.size() < 5 && !readyToWorkOn.empty()) {
            active.insert(*readyToWorkOn.begin());
            readyToWorkOn.erase(readyToWorkOn.begin());
        }

        // Get how much time should be advanced
        int firstJobDone = active.begin()->first;
        solution += firstJobDone;

        // Advance time
        set<pair<int, int> > activeTmp;
        for (auto it : active) {
            activeTmp.emplace(it.first - firstJobDone, it.second);
        }
        active = activeTmp;

        // Get more jobs after finishing the current one(s)
        while (!active.empty() && active.begin()->first <= 0) {
            pair<int, char> current = *active.begin();
            active.erase(active.begin());

            for (const auto &child : inDegree[current.second].second) {
                // If the previous nodes already went to the next step...
                // (equals 0 (and not <=) prevents adding the same node multiple times)
                if (--inDegree[child].first == 0) {
                    readyToWorkOn.emplace(child - 'A' + 1 + 60, child);
                }
            }
        }
    }

    cout << "Solution to part two = " << solution << endl;
}

int main() {
    cout << "--- The Sum of Its Parts ---" << endl;

    partOne();
    partTwo();

    return 0;
}
