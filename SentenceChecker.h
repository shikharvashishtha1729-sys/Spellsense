#ifndef SENTENCE_CHECKER_H
#define SENTENCE_CHECKER_H

#include <string>
class Trie;

// Checks each cleaned token in a sentence against the dictionary Trie.
void checkSentence(const Trie& dictionary, const std::string& sentence);

#endif
