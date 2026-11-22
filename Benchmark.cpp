#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Trie.h"
#include "Levenshtein.h"

using namespace std;
using Clock = chrono::steady_clock;
using Match = pair<string, int>;

// Baseline: compare the query against every dictionary word.
vector<Match> fullScan(const string& query, const vector<string>& words,
                       int maxDistance) {
    vector<Match> matches;
    for (const string& word : words) {
        int distance = editDistance(query, word);
        if (distance <= maxDistance) {
            matches.push_back({word, distance});
        }
    }
    sort(matches.begin(), matches.end());
    return matches;
}

vector<Match> prunedSearch(const Trie& trie, const string& query,
                           int maxDistance) {
    vector<Match> matches = trie.collectWordsWithinDistance(query, maxDistance);
    sort(matches.begin(), matches.end());
    return matches;
}

template <typename Function>
double averageMilliseconds(Function function, int repetitions) {
    // One warm-up run reduces first-run effects.
    function();
    auto start = Clock::now();
    for (int i = 0; i < repetitions; ++i) function();
    auto finish = Clock::now();
    return chrono::duration<double, milli>(finish - start).count() / repetitions;
}

int main() {
    const int MAX_DISTANCE = 2;
    const int REPETITIONS = 5;
    const vector<string> queries = {
        "wrld", "computr", "speling", "recieve", "langauge"
    };

    Trie dictionary;
    dictionary.loadDictionary("dictionary.txt");
    vector<string> allWords = dictionary.collectAllWords();
    cout << "Unique Trie words: " << allWords.size() << '\n';
    cout << "Edit-distance threshold: " << MAX_DISTANCE << '\n';
    cout << "Average of " << REPETITIONS << " timed runs per query\n\n";

    cout << left << setw(14) << "Query"
         << right << setw(14) << "Full scan(ms)"
         << setw(16) << "Pruned(ms)"
         << setw(13) << "Speedup"
         << setw(13) << "Matches" << '\n';
    cout << string(70, '-') << '\n';

    double totalFull = 0.0;
    double totalPruned = 0.0;

    for (const string& query : queries) {
        vector<Match> baseline = fullScan(query, allWords, MAX_DISTANCE);
        vector<Match> optimized = prunedSearch(dictionary, query, MAX_DISTANCE);

        if (baseline != optimized) {
            cerr << "ERROR: Results differ for query '" << query << "'.\n";
            return 1;
        }

        double fullMs = averageMilliseconds([&]() {
            volatile size_t count = fullScan(query, allWords, MAX_DISTANCE).size();
            (void)count;
        }, REPETITIONS);
        double prunedMs = averageMilliseconds([&]() {
            volatile size_t count = prunedSearch(dictionary, query, MAX_DISTANCE).size();
            (void)count;
        }, REPETITIONS);

        totalFull += fullMs;
        totalPruned += prunedMs;
        double speedup = prunedMs > 0.0 ? fullMs / prunedMs : 0.0;

        cout << left << setw(14) << query
             << right << fixed << setprecision(3)
             << setw(14) << fullMs
             << setw(16) << prunedMs
             << setw(12) << setprecision(2) << speedup << "x"
             << setw(13) << baseline.size() << '\n';
    }

    double averageSpeedup = totalPruned > 0.0 ? totalFull / totalPruned : 0.0;
    cout << string(70, '-') << '\n';
    cout << "Average full-scan time: " << fixed << setprecision(3)
         << totalFull / queries.size() << " ms\n";
    cout << "Average pruned-search time: " << totalPruned / queries.size() << " ms\n";
    cout << "Overall average speedup: " << setprecision(2)
         << averageSpeedup << "x\n";
    cout << "Correctness check: PASS (both methods returned identical matches)\n";
    cout << "Note: timings depend on your computer, compiler, and system load.\n";
    return 0;
}
