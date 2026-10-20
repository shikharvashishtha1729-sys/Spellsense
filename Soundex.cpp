#include "Soundex.h"
#include <cctype>
using namespace std;

string soundexCode(const string& word) {
    string cleaned;
    for (unsigned char ch : word) {
        if (isalpha(ch)) cleaned += char(toupper(ch));
    }
    if (cleaned.empty()) return "";

    auto codeFor = [](char ch) -> char {
        switch (ch) {
            case 'B': case 'F': case 'P': case 'V': return '1';
            case 'C': case 'G': case 'J': case 'K':
            case 'Q': case 'S': case 'X': case 'Z': return '2';
            case 'D': case 'T': return '3';
            case 'L': return '4';
            case 'M': case 'N': return '5';
            case 'R': return '6';
            default: return '0'; // vowels, H, W, Y
        }
    };

    string result;
    result += cleaned[0];
    char previousCode = codeFor(cleaned[0]);

    for (size_t i = 1; i < cleaned.size() && result.size() < 4; i++) {
        char currentCode = codeFor(cleaned[i]);

        // Vowels and H/W/Y separate adjacent groups; they are not output.
        if (currentCode == '0') {
            previousCode = '0';
        } else {
            if (currentCode != previousCode) result += currentCode;
            previousCode = currentCode;
        }
    }

    while (result.size() < 4) result += '0';
    return result;
}
