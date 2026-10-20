#ifndef SUGGESTION_ENGINE_H
#define SUGGESTION_ENGINE_H

#include <string>
#include <vector>

struct Suggestion {
    std::string word;
    int editDistance;
    bool phoneticMatch;
};

// Candidates are accepted only if edit distance <= maxDistance.
// Results prioritize smaller edit distance, then phonetic matches, then alphabetically.
std::vector<Suggestion> generateSuggestions(
    const std::string& wrongWord,
    const std::vector<std::string>& dictionaryWords,
    int maxDistance = 2
);

#endif
