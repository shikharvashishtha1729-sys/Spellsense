#include "Trie.h"
#include "WordUtils.h"

#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

TrieNode::TrieNode() : isEndOfWord(false) {
    for (int i = 0; i < 26; i++) {
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
        if (index < 0 || index >= 26) return;

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


void Trie::collectWordsWithinDistance(
    TrieNode* node,
    char addedCharacter,
    const string& currentWord,
    const string& targetWord,
    const vector<int>& previousRow,
    int maxDistance,
    vector<pair<string, int>>& results
) const {
    if (node == nullptr) return;

    int targetLength = static_cast<int>(targetWord.length());
    vector<int> currentRow(targetLength + 1);
    currentRow[0] = previousRow[0] + 1;

    int minimumInRow = currentRow[0];

    for (int j = 1; j <= targetLength; j++) {
        int cost = (targetWord[j - 1] == addedCharacter) ? 0 : 1;

        currentRow[j] = min({
            previousRow[j] + 1,       // deletion
            currentRow[j - 1] + 1,    // insertion
            previousRow[j - 1] + cost // match or substitution
        });

        if (currentRow[j] < minimumInRow) {
            minimumInRow = currentRow[j];
        }
    }

    // If even the best value in this row is too large, no word
    // below this Trie node can be within the allowed distance.
    if (minimumInRow > maxDistance) return;

    string updatedWord = currentWord + addedCharacter;

    if (node->isEndOfWord && currentRow[targetLength] <= maxDistance) {
        results.push_back({updatedWord, currentRow[targetLength]});
    }

    for (int i = 0; i < 26; i++) {
        if (node->children[i] != nullptr) {
            collectWordsWithinDistance(
                node->children[i],
                char('a' + i),
                updatedWord,
                targetWord,
                currentRow,
                maxDistance,
                results
            );
        }
    }
}

vector<pair<string, int>> Trie::collectWordsWithinDistance(
    const string& targetWord, int maxDistance
) const {
    vector<pair<string, int>> results;
    if (root == nullptr || maxDistance < 0) return results;

    vector<int> initialRow(targetWord.length() + 1);
    for (size_t j = 0; j <= targetWord.length(); j++) {
        initialRow[j] = static_cast<int>(j);
    }

    for (int i = 0; i < 26; i++) {
        if (root->children[i] != nullptr) {
            collectWordsWithinDistance(
                root->children[i],
                char('a' + i),
                "",
                targetWord,
                initialRow,
                maxDistance,
                results
            );
        }
    }

    return results;
}
