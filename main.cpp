#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <stack>
#include <algorithm>
#include <cctype>

#include "Trie.h"
#include "WordUtils.h"
#include "SentenceChecker.h"
#include "SuggestionEngine.h"

using namespace std;

struct SearchRecord {
    string word;
    vector<Suggestion> suggestions;
};

void displaySuggestions(const SearchRecord& record) {
    cout << "\nSuggestions for '" << record.word
         << "' (edit distance <= 2):\n";
    if (record.suggestions.empty()) {
        cout << "No suggestions found within edit distance 2.\n";
        return;
    }

    for (const Suggestion& suggestion : record.suggestions) {
        cout << suggestion.word
             << " | distance: " << suggestion.editDistance
             << " | Soundex match: "
             << (suggestion.phoneticMatch ? "Yes" : "No")
             << '\n';
    }
}

int main() {
    Trie dictionary;
    dictionary.loadDictionary("dictionary.txt");

    string sentence;
    cout << "\nEnter a sentence to spell-check: ";
    getline(cin, sentence);

    cout << "\nSpell Check Results:\n";
    checkSentence(dictionary, sentence);

    // Undo and redo stacks maintain the user's suggestion-search history.
    stack<SearchRecord> undoStack;
    stack<SearchRecord> redoStack;

    string wrongWord;
    while (true) {
        cout << "\nEnter a word, or type 'undo', 'redo', or 'exit': ";
        if (!(cin >> wrongWord)) {
            break;
        }

        string command = wrongWord;
        transform(command.begin(), command.end(), command.begin(),
                  [](unsigned char ch) { return static_cast<char>(tolower(ch)); });

        if (command == "exit") {
            cout << "Exiting SpellSense. Goodbye!\n";
            break;
        }

        if (command == "undo") {
            if (undoStack.empty()) {
                cout << "Nothing to undo.\n";
            } else {
                SearchRecord record = undoStack.top();
                undoStack.pop();
                redoStack.push(record);
                cout << "Undid the suggestion search for '" << record.word << "'.\n";
                if (!undoStack.empty()) {
                    cout << "Previous search in history: '" << undoStack.top().word << "'.\n";
                }
            }
            continue;
        }

        if (command == "redo") {
            if (redoStack.empty()) {
                cout << "Nothing to redo.\n";
            } else {
                SearchRecord record = redoStack.top();
                redoStack.pop();
                undoStack.push(record);
                cout << "Restored the suggestion search for '" << record.word << "'.\n";
                displaySuggestions(record);
            }
            continue;
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

        SearchRecord record;
        record.word = wrongWord;
        record.suggestions = generateSuggestions(wrongWord, dictionary, 2);

        // A new search creates a new history branch, so old redo actions are cleared.
        undoStack.push(record);
        while (!redoStack.empty()) redoStack.pop();

        displaySuggestions(record);
    }

    return 0;
}