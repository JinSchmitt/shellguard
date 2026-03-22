#include <iostream>

#include <iostream>
#include "trie.h"
#include "danger.h"
#include "corrector.h"

using namespace std;

int main(int argc, char* argv[] ) {

    Trie trie;
    unordered_map<string, int> history = loadHistory();

    if (argc < 2) {
        return 0;
    }

    bool rawMode = false;
    string arg;

    for (int i = 1; i < argc; i++) {
        string current = argv[i];

        if (current == "--raw") {
            rawMode = true;
            continue;
        }

        if (!arg.empty()) {
            arg += " ";
        }
        arg += current;
    }

    if (arg.empty()) {
        return 0;
    }

    vector<string> commands;
    loadCommandsFromPath(trie, commands);

    string cmd = arg.substr(0, arg.find(' '));
    string rest = arg.find(' ') != string::npos ? arg.substr(arg.find(' ')) : "";

    if (!trie.search(cmd)) {
        string suggestion = suggest(cmd, commands, history);
        if (!suggestion.empty()) {
            suggestion += rest;
            cout << suggestion << endl;

        }
    }

    if (isDangerous(cmd)) {
        cout << "⚠️  Dangerous command detected: " << arg << endl;
        cout << "Are you sure you want to run this? (y/n): " << endl;
    }



    return 0;
}