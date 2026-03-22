#pragma once
#include <string>
#include <vector>
#include "trie.h"

void loadCommandsFromPath(Trie& trie, std::vector<std::string>& commands);
std::string suggest(const std::string& input, const std::vector<std::string>& commands, const std::unordered_map<std::string, int>& history);
int levenshtein(const std::string& word1, const std::string& word2);
std::unordered_map<std::string, int> loadHistory();