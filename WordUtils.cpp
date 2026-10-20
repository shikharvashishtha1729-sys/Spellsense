#include "WordUtils.h"
#include <cctype>

using namespace std;

string sanitizeWord(const string& word) {
    string cleanedWord;

    for (unsigned char ch : word) {
        if (isalpha(ch)) {
            cleanedWord += char(tolower(ch));
        }
    }

    return cleanedWord;
}
