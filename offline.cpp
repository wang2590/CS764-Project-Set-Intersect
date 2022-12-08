#include <iostream>
#include <string>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <vector>

bool checkIntersection(const std::vector<std::pair<std::string, std::string>>& R,
                       std::unordered_map<std::string, int>& SS,
                       const std::string& setA, const std::string& setB) {
    std::unordered_set<std::string> eleB; // since SS[setA] >= SS[setB]

    for (const auto & n : R) {
        if (n.first == setB) {
            eleB.insert(n.second);
        }
    }
    for (const auto & n : R) {
        if (n.first == setA && eleB.find(n.second) != eleB.end()) {
            return true;
        }
    }
    return false;
}

std::unordered_map<std::string, std::unordered_map<std::string, bool>> materialize(
        const std::vector<std::pair<std::string, std::string>>& R,
        std::unordered_map<std::string, int>& SS, const int S) {

    // M['A']['B'] == False => no intersection for A and B
    std::unordered_map<std::string, std::unordered_map<std::string, bool>> M;
    double threshold = R.size() / sqrt(S);
    std::vector<std::string> preprocess;

    for (const std::pair<const std::string, int>& n : SS) {
        if (n.second > threshold) {
            preprocess.push_back(n.first);
        }
    }

    int N = preprocess.size();
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            std::string setA = preprocess[i];
            std::string setB = preprocess[j];

            if (SS[setA] < SS[setB]) {
                setB = preprocess[i];
                setA = preprocess[j];
            }
            bool intersect = checkIntersection(R, SS, setA, setB);
            M[setA][setB] = intersect;
            M[setB][setA] = intersect; // TODO: space optimization?
        }
    }

    return M;
}

std::unordered_map<std::string, int> buildSetSizeTable(const std::vector<std::pair<std::string, std::string>>& R) {
    std::unordered_map<std::string, int> SS; // table for set size

    for (const auto & n : R) {
        auto search = SS.find(n.first);
        if (search == SS.end()) {
            SS[n.first] = 0;
        }
        SS[n.first] += 1;
    }

    return SS;
}
