# SpellSense — Modular C++ Spell Checker

## Features in this version
- Loads dictionary words from `dictionary.txt`(We have used 'dictionary1.txt' for testing)
- Sanitizes words by keeping alphabetic characters and converting them to lowercase
- Stores words in a 26-way Trie and supports exact lookup
- Checks each word in a sentence
- Collects all dictionary words through recursive DFS
- Calculates Levenshtein edit distance using dynamic programming
- Implements classic four-character Soundex phonetic coding
- Generates suggestions if edit distance is <= 2 OR Soundex codes match
- Sorts suggestions by smaller edit distance, then phonetic match, then alphabetically
- Frees Trie nodes with a recursive destructor

## Files and responsibilities
- `main.cpp` — user interaction and connects modules
- `Trie.h`, `Trie.cpp` — Trie nodes, insertion, exact lookup, dictionary loading, DFS collection, cleanup
- `WordUtils.h`, `WordUtils.cpp` — word sanitization
- `SentenceChecker.h`, `SentenceChecker.cpp` — sentence tokenization and spelling checks
- `Levenshtein.h`, `Levenshtein.cpp` — edit-distance DP
- `Soundex.h`, `Soundex.cpp` — Soundex code generation
- `SuggestionEngine.h`, `SuggestionEngine.cpp` — combines edit distance and Soundex for suggestions
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

## Current search approach and limitation
This version still uses the baseline suggestion method:
1. Collect all dictionary words from the Trie using DFS.
2. Compute Levenshtein distance for every word and compare Soundex codes.
3. Include a candidate if its edit distance is <= 2 or its Soundex code matches.
4. Sort candidates by edit distance, then phonetic match, then word.

Trie-based branch-pruned Levenshtein and Min-Heap top-k ranking are not implemented in this version. The suggestions are sorted with `std::sort`, not a custom Min-Heap.


