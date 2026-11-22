# SpellSense — Modular C++ Spell Checker

## Features in this version
- Loads dictionary words from `dictionary.txt`(We have used 'dictionary1.txt' for testing)
- Sanitizes words by keeping alphabetic characters and converting them to lowercase
- Stores words in a 26-way Trie and supports exact lookup
- Checks each word in a sentence
- Collects all dictionary words through recursive DFS
- Calculates Levenshtein edit distance using dynamic programming
- Implements classic four-character Soundex phonetic coding
- Generates suggestions only when Levenshtein edit distance is <= 2; Soundex is used as a ranking tie-breaker
- Uses a custom Min-Heap to return the top 10 suggestions, ordered by smaller edit distance, then phonetic match, then alphabetically
- Supports Undo/Redo of suggestion searches using two stacks (a new search clears redo history)
- Frees Trie nodes with a recursive destructor

## Files and responsibilities
- `main.cpp` — user interaction, suggestion-search history, and Undo/Redo using two stacks
- `Trie.h`, `Trie.cpp` — Trie nodes, insertion, exact lookup, dictionary loading, DFS collection, cleanup
- `WordUtils.h`, `WordUtils.cpp` — word sanitization
- `SentenceChecker.h`, `SentenceChecker.cpp` — sentence tokenization and spelling checks
- `Levenshtein.h`, `Levenshtein.cpp` — edit-distance DP
- `Soundex.h`, `Soundex.cpp` — Soundex code generation
- `SuggestionEngine.h`, `SuggestionEngine.cpp` — combines edit distance and Soundex, builds a custom Min-Heap, and extracts the top 10 suggestions
- `dictionary.txt` — the team's full dictionary
- `README.md` — build instructions and module guide

## Compile with GCC / G++
Run from this folder:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp Trie.cpp WordUtils.cpp SentenceChecker.cpp Levenshtein.cpp Soundex.cpp SuggestionEngine.cpp -o SpellSense
```

On Windows with MinGW, run:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp Trie.cpp WordUtils.cpp SentenceChecker.cpp Levenshtein.cpp Soundex.cpp SuggestionEngine.cpp -o SpellSense.exe
```

Keep `dictionary.txt` in the working directory when running.

## Current search approach
1. Traverse the Trie using Levenshtein DP rows.
2. Prune a Trie branch when the minimum value in its DP row exceeds the maximum allowed edit distance.
3. Calculate Soundex matches for the remaining candidates.
4. Build a custom Min-Heap using the ranking order: smaller edit distance first, Soundex matches first when distances tie, then alphabetical order.
5. Extract at most the top 10 suggestions from the heap.

The Min-Heap is built bottom-up in O(R) time for R qualifying candidates. Extracting K suggestions takes O(K log R) time. The heap stores O(R) suggestions.


## Undo and Redo
- Every misspelled-word suggestion search is stored as a `SearchRecord` containing the query and its generated suggestions.
- The `undoStack` stores the active search history; `undo` moves the latest search to `redoStack`.
- `redo` moves the most recently undone search back to `undoStack` and displays its saved suggestions.
- Performing a new suggestion search clears `redoStack`, because the user has started a new history sequence.
- These commands undo/redo suggestion-search history; they do not edit dictionary entries.

## Performance benchmarking
A separate benchmark compares the original full-dictionary scan (run Levenshtein DP against every Trie word) with the optimized Trie traversal that prunes branches using DP rows. It checks that both approaches return exactly the same matching words and edit distances before reporting timings.

Compile the benchmark separately (do not include `main.cpp`, because both files contain `main()`):

```bash
g++ -std=c++17 -O2 -Wall -Wextra Benchmark.cpp Trie.cpp WordUtils.cpp Levenshtein.cpp -o SpellSenseBenchmark
```

Run it from this folder so `dictionary.txt` is found:

```bash
./SpellSenseBenchmark
```

On Windows with MinGW:

```bash
g++ -std=c++17 -O2 -Wall -Wextra Benchmark.cpp Trie.cpp WordUtils.cpp Levenshtein.cpp -o SpellSenseBenchmark.exe
SpellSenseBenchmark.exe
```

The benchmark uses five representative queries and reports average milliseconds over five timed runs. Results are machine-dependent; use the measurements produced on the computer used for your evaluation rather than copying sample timings.



