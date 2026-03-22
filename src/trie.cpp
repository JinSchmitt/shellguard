#include <iostream>
#include <unordered_map>
#include <memory>
#include "trie.h"
using namespace std;

// Inserts a word into the trie by walking character by character,
// creating new nodes where the path doesn't exist yet
void Trie::insert(const std::string& word) {
    TrieNode* current = &root;

    // For each character in the word, check if a child node exists.
    // If not, create one. Then move current down to that child.
    for (char c : word) {
        if (current->children.count(c) == 0) {
            current->children[c] = make_unique<TrieNode>();
        }
        // Advance current to the next node in the path
        current = current->children[c].get();
    }
    // Mark the final node as the end of a valid word
    current->is_end = true;
}

// Returns true if the exact word exists in the trie, false otherwise
bool Trie::search(const std::string& word) const {
    // const pointer required since this is a const function
    const TrieNode* current = &root;

    for (char c : word) {
        // If the character has no node, the word doesn't exist
        if (current->children.count(c) == 0) {
            return false;
        }
        // Use .at() instead of [] for const-safe map access
        current = current->children.at(c).get();
    }
    // Word exists only if the final node is marked as a word ending
    return current->is_end;
}

// Returns true if any word in the trie begins with the given prefix.
// Unlike search, does not require a word to end at the final node.
bool Trie::starts_with(const std::string &prefix) const {
    const TrieNode* current = &root;
    for (char c : prefix) {
        // If the character has no node, no word with this prefix exists
        if (current->children.count(c) == 0) {
            return false;
        }
        current = current->children.at(c).get();
    }
    // If we walked the full prefix without missing a node, it exists
    return true;
}