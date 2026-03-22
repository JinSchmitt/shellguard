#include <iostream>
#include <memory>
#include <unistd.h>
#include <filesystem>
#include <string>
#include <sstream>
#include <fstream>
#include "trie.h"
#include "corrector.h"

using namespace std;

// Scans all directories in the system $PATH and inserts every
// executable file found into the trie as a valid command
void loadCommandsFromPath(Trie& trie, vector<string>& commands) {
    // Get the PATH environment variable as a raw C string
    const char* path = getenv("PATH");

    // If PATH doesn't exist, nothing to load
    if (path == nullptr) return;

    // Convert to std::string for easier manipulation
    string pathStr = string(path);

    // Use stringstream to split PATH by ':' into individual directories
    stringstream ss(pathStr);
    string dir;
    while (getline(ss, dir, ':')) {
        // dir is one directory path each iteration
        try {
            // Iterate over every file in this directory
            for (const auto& entry : std::filesystem::directory_iterator(dir)) {
                // Only insert if it's a regular file and executable by the current user
                if (entry.is_regular_file() && access(entry.path().c_str(), X_OK) == 0) {
                    //insert just the file name into the trie
                   trie.insert(entry.path().filename().c_str());
                   commands.push_back(entry.path().filename().c_str());
                }
            }
        } catch (...) {}
    }

}
int levenshtein(const string& word1, const string& word2) {
    int size1 = word1.size();
    int size2 = word2.size();
    int verif[size1 + 1][size2 + 1]; // Verification matrix i.e. 2D array which will store the calculated distance.

    // If one of the words has zero length, the distance is equal to the size of the other word.
    if (size1 == 0)
        return size2;
    if (size2 == 0)
        return size1;

    // Sets the first row and the first column of the verification matrix with the numerical order from 0 to the length of each word.
    for (int i = 0; i <= size1; i++)
        verif[i][0] = i;
    for (int j = 0; j <= size2; j++)
        verif[0][j] = j;

    // Verification step / matrix filling.
    for (int i = 1; i <= size1; i++) {
        for (int j = 1; j <= size2; j++) {
            // Sets the modification cost.
            // 0 means no modification (i.e. equal letters) and 1 means that a modification is needed (i.e. unequal letters).
            int cost = (word2[j - 1] == word1[i - 1]) ? 0 : 1;

            // Sets the current position of the matrix as the minimum value between a (deletion), b (insertion) and c (substitution).
            // a = the upper adjacent value plus 1: verif[i - 1][j] + 1
            // b = the left adjacent value plus 1: verif[i][j - 1] + 1
            // c = the upper left adjacent value plus the modification cost: verif[i - 1][j - 1] + cost
            verif[i][j] = min(
                min(verif[i - 1][j] + 1, verif[i][j - 1] + 1),
                verif[i - 1][j - 1] + cost
            );
        }
    }

    // The last position of the matrix will contain the Levenshtein distance.
    return verif[size1][size2];
}

string suggest(const string& input, const vector<string>& commands, const unordered_map<string, int>& history) {
    if (input.empty()) return "";

    string bestMatch = "";
    int bestDist = INT_MAX;


    for (const string& word : commands) {
        if (word != input) {
            int dist = levenshtein(word, input);
            if (dist > 3) {
                continue;
            }
            if (word[0] != input[0]) {
                continue;
            }
            int freq = history.count(word) ? history.at(word) : 0;
            int adjustedDist = dist - min(freq / 50, 2);
            if (adjustedDist < bestDist) {
                bestMatch = word;
                bestDist = adjustedDist;
            }
        }
    }
    return bestMatch;
}

unordered_map<string, int> loadHistory() {
    unordered_map<string, int> history;
    string path = string(getenv("HOME")) + "/.zsh_history";
    ifstream zsh_file(path);
    string line;
    while (getline(zsh_file, line)) {
        int pos = line.find(';');
        if (pos == string::npos) continue;  // skip lines with no semicolon
        string full_cmd = line.substr(pos + 1);
        string cmd = full_cmd.substr(0, full_cmd.find(' ')); // grab first word
        if (!cmd.empty()) {
            history[cmd]++;  // increment count
        }
    }
    return history;  // return the map
}