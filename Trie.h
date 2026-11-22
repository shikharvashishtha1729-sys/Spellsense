#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include <utility>

struct TrieNode {
    TrieNode* children[26];
    bool isEndOfWord;
    TrieNode();
};

class Trie {
private:
    TrieNode* root;
    void collectWords(TrieNode* node, const std::string& currentWord,
                      std::vector<std::string>& words) const;
    void deleteTrie(TrieNode* node);
    void collectWordsWithinDistance(
        TrieNode* node,
        char addedCharacter,
        const std::string& currentWord,
        const std::string& targetWord,
        const std::vector<int>& previousRow,
        int maxDistance,
        std::vector<std::pair<std::string, int>>& results
    ) const;

public:
    Trie();
    ~Trie();

    // This Trie stores lowercase English words (a-z).
    void insert(const std::string& word);
    bool search(const std::string& word) const;
    void loadDictionary(const std::string& filename);
    std::vector<std::string> collectAllWords() const;
    // Uses Levenshtein DP rows to prune Trie branches that cannot meet maxDistance.
    std::vector<std::pair<std::string, int>> collectWordsWithinDistance(
        const std::string& targetWord, int maxDistance
    ) const;
};

#endif
