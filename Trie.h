#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>

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

public:
    Trie();
    ~Trie();

    // This Trie stores lowercase English words (a-z).
    void insert(const std::string& word);
    bool search(const std::string& word) const;
    void loadDictionary(const std::string& filename);
    std::vector<std::string> collectAllWords() const;
};

#endif
