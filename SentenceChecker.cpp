#include "SentenceChecker.h"
#include "Trie.h"
#include "WordUtils.h"

#include <iostream>
#include <sstream>
using namespace std;

void checkSentence(const Trie& dictionary, const string& sentence) {
    stringstream input(sentence);
    string word;

    while (input >> word) {
        string cleanedWord = sanitizeWord(word);
        if (cleanedWord.empty()) continue;

        if (dictionary.search(cleanedWord)) {
            cout << cleanedWord << " -> Correct" << endl;
        } else {
            cout << cleanedWord << " -> Misspelled" << endl;
        }
    }
}
