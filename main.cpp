#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include "Trie.h"
#include "WordUtils.h"
#include "SentenceChecker.h"
#include "SuggestionEngine.h"

using namespace std;

int main() {
    Trie dictionary;
    dictionary.loadDictionary("dictionary.txt");

    string sentence;
    cout << "\nEnter a sentence to spell-check: ";
    getline(cin, sentence);

    cout << "\nSpell Check Results:\n";
    checkSentence(dictionary, sentence);

    // Collect dictionary words once; reuse them for every suggestion search.
    vector<string> dictionaryWords = dictionary.collectAllWords();

    string wrongWord;
    while (true) {
        cout << "\nEnter a misspelled word for suggestions (or type 'exit' to finish): ";
        if (!(cin >> wrongWord)) {
            break;
        }

        if (wrongWord == "exit" || wrongWord == "EXIT" || wrongWord == "Exit") {
            cout << "Exiting SpellSense. Goodbye!\n";
            break;
        }

        wrongWord = sanitizeWord(wrongWord);
        if (wrongWord.empty()) {
            cout << "Please enter a word containing letters.\n";
            continue;
        }

        if (dictionary.search(wrongWord)) {
            cout << "'" << wrongWord << "' is already in the dictionary.\n";
            continue;
        }

        vector<Suggestion> results =
            generateSuggestions(wrongWord, dictionaryWords, 2);

        cout << "\nSuggestions (edit distance <= 2):\n";
        if (results.empty()) {
            cout << "No suggestions found within edit distance 2.\n";
        } else {
            for (const Suggestion& suggestion : results) {
                cout << suggestion.word
                     << " | distance: " << suggestion.editDistance
                     << " | Soundex match: "
                     << (suggestion.phoneticMatch ? "Yes" : "No")
                     << '\n';
            }
        }
    }

    return 0;
}
