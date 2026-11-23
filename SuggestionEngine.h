#ifndef SUGGESTION_ENGINE_H
#define SUGGESTION_ENGINE_H

#include <string>
#include <vector>
#include "Trie.h"

struct Suggestion {
    std::string word;
    int editDistance;
    bool phoneticMatch;
};

// Candidates are accepted only if edit distance <= maxDistance.
// Results prioritize smaller edit distance, phonetic matches, then alphabetically.
std::vector<Suggestion> generateSuggestions(
    const std::string& wrongWord,
    const Trie& dictionary,
    int maxDistance = 2
);

#endif
