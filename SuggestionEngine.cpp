#include "SuggestionEngine.h"
#include "Levenshtein.h"
#include "Soundex.h"

#include <utility>
#include <vector>
using namespace std;

namespace {
// Returns true when a should appear before b in the final suggestion list.
bool betterSuggestion(const Suggestion& a, const Suggestion& b) {
    if (a.editDistance != b.editDistance)
        return a.editDistance < b.editDistance;
    if (a.phoneticMatch != b.phoneticMatch)
        return a.phoneticMatch > b.phoneticMatch;
    return a.word < b.word;
}


class SuggestionMinHeap {
private:
    vector<Suggestion> heap;

    void siftDown(int index) {
        int size = static_cast<int>(heap.size());
        while (true) {
            int smallest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            if (left < size && betterSuggestion(heap[left], heap[smallest]))
                smallest = left;
            if (right < size && betterSuggestion(heap[right], heap[smallest]))
                smallest = right;

            if (smallest == index) break;
            swap(heap[index], heap[smallest]);
            index = smallest;
        }
    }

public:
    explicit SuggestionMinHeap(vector<Suggestion> suggestions)
        : heap(std::move(suggestions)) {
        // Bottom-up heap construction takes O(n) time.
        for (int i = static_cast<int>(heap.size()) / 2 - 1; i >= 0; --i)
            siftDown(i);
    }

    bool empty() const {
        return heap.empty();
    }

    Suggestion extractMin() {
        Suggestion best = heap.front();
        heap.front() = heap.back();
        heap.pop_back();
        if (!heap.empty()) siftDown(0);
        return best;
    }
};
} // namespace

vector<Suggestion> generateSuggestions(const string& wrongWord,
                                       const Trie& dictionary,
                                       int maxDistance) {
    vector<Suggestion> candidates;
    string targetCode = soundexCode(wrongWord);

    // Trie traversal calculates edit distances and prunes unpromising branches.
    vector<pair<string, int>> matchingWords =
        dictionary.collectWordsWithinDistance(wrongWord, maxDistance);

    for (const auto& candidate : matchingWords) {
        bool phoneticMatch = !targetCode.empty()
                          && targetCode == soundexCode(candidate.first);
        candidates.push_back({candidate.first, candidate.second, phoneticMatch});
    }

    // Build the Min-Heap and extract suggestions from best to worst.
    SuggestionMinHeap minHeap(std::move(candidates));
    vector<Suggestion> results;
    const size_t TOP_SUGGESTIONS = 10;

    while (!minHeap.empty() && results.size() < TOP_SUGGESTIONS) {
        results.push_back(minHeap.extractMin());
    }

    return results;
}

