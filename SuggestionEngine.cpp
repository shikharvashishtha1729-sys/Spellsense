#include "SuggestionEngine.h"
#include "Levenshtein.h"
#include "Soundex.h"

#include <algorithm>
using namespace std;

vector<Suggestion> generateSuggestions(const string& wrongWord,
                                       const vector<string>& dictionaryWords,
                                       int maxDistance) {
    vector<Suggestion> results;
    string targetCode = soundexCode(wrongWord);

    for (const string& candidate : dictionaryWords) {
        int distance = editDistance(wrongWord, candidate);
        bool phoneticMatch = !targetCode.empty()
                          && targetCode == soundexCode(candidate);

        if (distance <= maxDistance) {
            results.push_back({candidate, distance, phoneticMatch});
        }
    }

    sort(results.begin(), results.end(),
         [](const Suggestion& a, const Suggestion& b) {
             if (a.editDistance != b.editDistance)
                 return a.editDistance < b.editDistance;
             if (a.phoneticMatch != b.phoneticMatch)
                 return a.phoneticMatch > b.phoneticMatch;
             return a.word < b.word;
         });

    return results;
}
