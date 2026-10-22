#include "Trie.h"
#include "WordUtils.h"
#include <iostream>
#include <fstream>
using namespace std;

TrieNode::TrieNode() : isEndOfWord(false) {
    for (int i = 0; i < 26; i++){
        children[i] = nullptr;
    }
}

Trie::Trie() : root(new TrieNode()) {}

Trie::~Trie() {
    deleteTrie(root);
}

void Trie::deleteTrie(TrieNode* node) {
    if (node == nullptr) return;

    for (int i = 0; i < 26; i++) {
        deleteTrie(node->children[i]);
    }
    delete node;
}

void Trie::insert(const string& word) {
    TrieNode* current = root;

    for (char ch : word) {
        int index = ch - 'a';
        if (index < 0 || index >= 26) return ;

        if (current->children[index] == nullptr) {
            current->children[index] = new TrieNode();
        }
        current = current->children[index];
    }

    if (!word.empty()) current->isEndOfWord = true;
}

bool Trie::search(const string& word) const {
    TrieNode* current = root;

    for (char ch : word) {
        int index = ch - 'a';
        if (index < 0 || index >= 26) return false;
        if (current->children[index] == nullptr) return false;
        current = current->children[index];
    }

    return !word.empty() && current->isEndOfWord;
}

void Trie::loadDictionary(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Error: Could not open dictionary file: "
             << filename << endl;
        return;
    }

    string word;
    int wordCount = 0;
    while (file >> word) {
        string cleanedWord = sanitizeWord(word);
        if (!cleanedWord.empty()) {
            insert(cleanedWord);
            wordCount++;
        }
    }

    cout << wordCount << " dictionary entries loaded." << endl;
}

void Trie::collectWords(TrieNode* node, const string& currentWord,
                        vector<string>& words) const {
    if (node == nullptr) return;

    if (node->isEndOfWord) words.push_back(currentWord);

    for (int i = 0; i < 26; i++) {
        if (node->children[i] != nullptr) {
            char ch = char('a' + i);
            collectWords(node->children[i], currentWord + ch, words);
        }
    }
}

vector<string> Trie::collectAllWords() const {
    vector<string> words;
    collectWords(root, "", words);
    return words;
}
