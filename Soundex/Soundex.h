#ifndef SOUNDEX_H
#define SOUNDEX_H

#include <string>

// Returns a classic four-character Soundex code, e.g. phone -> P500.
std::string soundexCode(const std::string& word);

#endif
