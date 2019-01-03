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
#include <unordered_set>
#include <iomanip>
#include <regex>
#include <sstream>

using namespace std;

class Group {
private:
    static int idGenerator;
    int id, nUnits, nHitPoints, nDamage, nInitiative;
    string damageType;
    bool isInfection;
    unordered_set<string> immune, weakness;

public:
    explicit Group(string line, const bool &isInfection, const int &boost = 0) {
        id = idGenerator++;
        this->isInfection = isInfection;

        if (line.find('(') != string::npos) {
            // It contains strength and/or weakness

            string::size_type start = line.find('(') + 1;

            string::size_type length = line.find(')') - start;

            string sw = line.substr(start, length);

            parse(immune, sw.c_str(), ".*immune to ([\\w, ]+);?.*");
            parse(weakness, sw.c_str(), ".*weak to ([\\w, ]+);?.*");

            line.erase(start-1, length + 3); // remove also the space
        }

        char damageType[1024];

        const char *format = "%d units each with %d hit points with an attack that does %d %s damage at initiative %d";

        sscanf(line.c_str(), format, &nUnits, &nHitPoints, &nDamage, damageType, &nInitiative);

        nDamage += boost;

        this->damageType = damageType;
    }

    void parse(unordered_set<string> &uss, const char expr[], const string &regexExp) {
        cmatch m;
        regex_match (expr, m, regex(regexExp));

        if (m.size() > 1) {
            string types = m[1];

            types.erase(remove(types.begin(), types.end(), ','), types.end());

            istringstream iss(types);
            uss = unordered_set<string>(istream_iterator<string>{iss},
                                        istream_iterator<string>());
        }
    }

    bool isDead() const {
        return nUnits <= 0;
    }

    int getEffectivePower() const {
        return nUnits * nDamage;
    }

    int getNumberOfUnits() const {
        return nUnits;
    }

    bool operator<(const Group &rhs) const {
        if (getEffectivePower() != rhs.getEffectivePower()) {
            return getEffectivePower() < rhs.getEffectivePower();
        }

        return nInitiative < rhs.nInitiative;
    }

    bool operator>(const Group &rhs) const {
        if (getEffectivePower() != rhs.getEffectivePower()) {
            return getEffectivePower() > rhs.getEffectivePower();
        }

        return nInitiative > rhs.nInitiative;
    }

    bool operator==(const Group &rhs) const {
        return id == rhs.id;
    }

    int simulateBeingAttackedBy(const Group &g) {
        if (immune.find(g.damageType) != immune.end()) {
            return 0;
        }

        int damage = g.getEffectivePower();

        if (weakness.find(g.damageType) != weakness.end()) {
            damage *= 2;
        }

        return damage;
    }

    void getAttacked(const int &attackDamage) {
        nUnits -= attackDamage / nHitPoints;
    }

    bool isInfectionType() const {
        return isInfection;
    }

    int getInitiative() const {
        return nInitiative;
    }
};

int Group::idGenerator = 0;

class BattleGround {
private:
    vector<Group> armies;
public:
    void readFile(const string &filename, const int &boost = 0) {
        ifstream fin(filename);

        string line;

        // Immune system
        fin.ignore(1000, '\n');
        while (getline(fin, line) && !line.empty()) {
            armies.push_back(Group(line, false, boost));
        }

        // Infection
        fin.ignore(1000, '\n');
        while (getline(fin, line) && !line.empty()) {
            armies.push_back(Group(line, true));
        }
    }

    bool isItOver() const {
        bool isImmuneAlive = false, isInfectionAlive = false;

        for (const Group &g : armies) {
            if (g.isInfectionType()) {
                bool res = !g.isDead();
                isInfectionAlive = isInfectionAlive || !g.isDead();
            } else {
                bool res = !g.isDead();
                isImmuneAlive = isImmuneAlive || !g.isDead();
            }
        }

        return !(isImmuneAlive && isInfectionAlive);
    }

    int countUnits() const {
        int total = 0;

        for (const Group &g : armies) {
            total += max(0, g.getNumberOfUnits());
        }

        return total;
    }

    bool didImmuneSystemWin() {
        int total = 0;

        for (const Group &g : armies) {
            if (g.isInfectionType()) {
                total += max(0, g.getNumberOfUnits());
            }
        }

        return total <= 0;
    }

    bool fight() {
        auto groupPQComp =
                [&](const int &lhs, const int &rhs)
                { return armies.at(lhs) < armies.at(rhs); };
        auto groupSComp =
                [&](const int &lhs, const int &rhs)
                { return armies.at(lhs) > armies.at(rhs); };

        // Target selection
        priority_queue<int, vector<int>, decltype(groupPQComp)> attackers(groupPQComp);
        set<int, decltype(groupSComp)> victims(groupSComp);

        for(int i = 0; i < armies.size(); i++) {
            if (!armies.at(i).isDead()) {
                attackers.push(i);
                victims.insert(i);
            }
        }

        // Pair Attacker - Victim
        auto attackersComp =
                [&](const int lhs, const int &rhs)
                { return armies.at(lhs).getInitiative() > armies.at(rhs).getInitiative(); };
        map<int, int, decltype(attackersComp)> attackerVictimGroups(attackersComp);

        while (!attackers.empty()) {
            const int attackerIdx = attackers.top();
            const Group &attacker = armies.at(attackerIdx);
            attackers.pop();

            int bestAttack = 0, currentDamage;
            set<int>::iterator bestVictimIt;

            for (auto it = victims.begin(); it != victims.end(); it++) {
                const int victimIdx = *it;

                // Skip the victim if they are on the same team
                if (attacker.isInfectionType() == armies.at(victimIdx).isInfectionType()) {
                    continue;
                }

                currentDamage = armies.at(victimIdx).simulateBeingAttackedBy(attacker);

                if (currentDamage > bestAttack) {
                    bestAttack = currentDamage;
                    bestVictimIt = it;
                }
            }

            if (bestAttack > 0) {
                attackerVictimGroups[attackerIdx] = *bestVictimIt;

                victims.erase(bestVictimIt);
            }
        }

        int unitsBeforeAttack = countUnits();

        // Attack
        for (auto p : attackerVictimGroups) {
            const Group &attacker = armies.at(p.first);
            Group &victim = armies.at(p.second);

            if (!attacker.isDead() && !victim.isDead()) {
                int damage = victim.simulateBeingAttackedBy(attacker);
                victim.getAttacked(damage);
            }
        }

        return unitsBeforeAttack != countUnits();
    }
};

void partOne() {
    BattleGround bg;

    bg.readFile("input");

    while (!bg.isItOver()) {
        bg.fight();
    }

    cout << "Solution to part one = " << bg.countUnits() << endl;
}

void partTwo() {
    for (int boost = 0; ; boost++) {
        BattleGround bg;

        bg.readFile("input", boost);

        while (!bg.isItOver() && bg.fight());

        if (bg.didImmuneSystemWin()) {
            cout << "Solution to part two = " << bg.countUnits() << endl;
            return;
        }
    }
}

int main() {
    cout << "--- Immune System Simulator 20XX ---" << endl;

    partOne();
    partTwo();

    return 0;
}
