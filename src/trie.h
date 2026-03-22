// src/trie.h
#pragma once
#include <string>
#include <unordered_map>
#include <memory>

struct TrieNode {
    std::unordered_map<char, std::unique_ptr<TrieNode>> children;
    bool is_end = false;
};

class Trie {
public:
    void insert(const std::string& word);
    bool search(const std::string& word) const;
    bool starts_with(const std::string& prefix) const;

private:
    TrieNode root;
};