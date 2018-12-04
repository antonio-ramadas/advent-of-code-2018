#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

void partOne() {
    ifstream fin("input");

    string line, action;
    int activity;
    map<string, int> m;

    // Read
    while (getline(fin, line)) {
        action = line.substr(19);

        if (action == "wakes up") {
            activity = -1;
        } else if (action == "falls asleep") {
            activity = -2;
        } else {
            sscanf(action.c_str(), "Guard #%d begins shift", &activity);
        }

        m[line.substr(1, 16)] = activity;
    }

    int currentGuard = 0, startSleep = 0;
    int bestGuard = 0, maxMinutesSleeping = 0;
    map<int, vector<int> > sleepSchedule;
    map<int, int> minutesSleeping;

    // Sort and track sleep
    for (const auto &it : m) {
        if (it.second >= 0) {
            currentGuard = it.second;
        } else if (it.second == -2) {
            // Get the minutes
            startSleep = stoi(it.first.substr(it.first.size() - 2));
        } else {
            int endSleep = stoi(it.first.substr(it.first.size() - 2));

            minutesSleeping[currentGuard] += endSleep - startSleep;

            vector<int> &vec = sleepSchedule[currentGuard];

            vec.resize(60);

            for(; startSleep < endSleep; startSleep++) {
                vec.at(startSleep)++;
            }
        }
    }

    fin.close();

    maxMinutesSleeping = 0;

    // Count minutes the guard is sleeping
    for (const auto &it : minutesSleeping) {
        if (it.second > maxMinutesSleeping) {
            bestGuard = it.first;
            maxMinutesSleeping = it.second;
        }
    }

    maxMinutesSleeping = 0;

    // Get the best minute
    for (int i = 1; i < sleepSchedule[bestGuard].size(); i++) {
        if (sleepSchedule[bestGuard].at(i) > sleepSchedule[bestGuard].at(maxMinutesSleeping)) {
            maxMinutesSleeping = i;
        }
    }

    cout << "Solution to part one = " << bestGuard * maxMinutesSleeping << endl;
}

void partTwo() {
    ifstream fin("input");

    string line, action, prefixExample = "[1518-09-01 00:58] ";
    int activity, year, month, day, hour, minute;
    map<string, int> m;

    // Read
    while (getline(fin, line)) {
        action = line.substr(19);

        if (action == "wakes up") {
            activity = -1;
        } else if (action == "falls asleep") {
            activity = -2;
        } else {
            sscanf(action.c_str(), "Guard #%d begins shift", &activity);
        }

        m[line.substr(1, 16)] = activity;
    }

    int currentGuard = 0, startSleep = 0, bestGuard = 0;
    map<int, vector<int> > sleepSchedule;
    map<int, int> minutesSleeping;

    // Sort and track sleep
    for (const auto &it : m) {
        if (it.second >= 0) {
            currentGuard = it.second;
        } else if (it.second == -2) {
            // Get the minutes
            startSleep = stoi(it.first.substr(it.first.size() - 2));
        } else {
            int endSleep = stoi(it.first.substr(it.first.size() - 2));

            minutesSleeping[currentGuard] += endSleep - startSleep;

            vector<int> &vec = sleepSchedule[currentGuard];

            vec.resize(60);

            for(; startSleep < endSleep; startSleep++) {
                vec.at(startSleep)++;
            }
        }
    }

    fin.close();

    int bestMinute = 0;
    bestGuard = -1;

    for (const auto &it : sleepSchedule) {
        for (int minuteIndex = 0; minuteIndex < it.second.size(); minuteIndex++) {
            if (bestGuard == -1 || it.second.at(minuteIndex) > sleepSchedule.at(bestGuard).at(bestMinute)) {
                bestGuard = it.first;
                bestMinute = minuteIndex;
            }
        }
    }

    cout << "Solution to part one = " << bestGuard * bestMinute << endl;
}

int main() {
    cout << "--- Repose Record ---" << endl;

    partOne();
    partTwo();

    return 0;
}
