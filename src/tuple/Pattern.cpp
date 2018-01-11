#include <vector>
#include <sstream>
#include <cstring>
#include <functional>
#include <regex>
#include <iostream>
#include "Pattern.h"
#include "Tuple.h"
#include "limits.h"

bool isPattern(const std::string &s) {
    return std::regex_match(s, std::regex(PATTERN_REGEX));
}

Pattern::Pattern() {}

Pattern::Pattern(const std::string &pattern) : pattern(pattern) {
    if(pattern.length()+1 > PIPE_BUF-9) {
        throw std::invalid_argument("String too long for pipe");
    }
    if (!isPattern(pattern)) {
        throw std::invalid_argument("String is not a valid pattern");
    }
}

Pattern::~Pattern() {

}

std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        if (token[0] == ' ') token.erase(0, 1);
        if (token.length() > 0)
            tokens.push_back(token);
    }
    return tokens;
}

// returns comparison function and pattern stripped down to only the value
std::tuple<std::function<bool(int, int)>, std::string> patternToIntComparison(std::string pattern) {
    std::function<bool(int, int)> comparison = [](int j, int k) { return j == k; };

    if (pattern[0] == '=' && pattern[1] == '=') {
        pattern.erase(0, 2);
    } else if (pattern[0] == '>') {
        pattern.erase(0, 1);
        comparison = [](int j, int k) { return j > k; };
        if (pattern[0] == '=') {
            pattern.erase(0, 1);
            comparison = [](int j, int k) { return j >= k; };
        }
    } else if (pattern[0] == '<') {
        pattern.erase(0, 1);
        comparison = [](int j, int k) { return j < k; };
        if (pattern[0] == '=') {
            pattern.erase(0, 1);
            comparison = [](int j, int k) { return j <= k; };
        }
    } else if (pattern[0] == '*') {
        pattern.erase(0, 1);
        if (pattern.length() == 0) pattern.push_back('0'); // ugly
        comparison = [](int j, int k) { return true; };
    } else if (pattern[0] < '0') throw std::invalid_argument("Invalid pattern");

    return std::make_tuple(comparison, pattern);
}

// returns comparison function, pattern stripped down to only the value
// and a bool that is true if it's an inequality comparison
std::tuple<std::function<bool(char, char)>, std::string, bool> patternToStringComparison(std::string pattern) {
    bool lexi = false;
    std::function<bool(char, char)> comparison = [](char j, char k) { return j == k; };

    if (pattern[0] == '>') {
        lexi = true;
        pattern.erase(0, 1);
        comparison = [](char j, char k) { return j > k; };
        if (pattern[0] == '=') {
            pattern.erase(0, 1);
            comparison = [](char j, char k) { return j >= k; };
        }
    } else if (pattern[0] == '<') {
        lexi = true;
        pattern.erase(0, 1);
        comparison = [](char j, char k) { return j < k; };
        if (pattern[0] == '=') {
            pattern.erase(0, 1);
            comparison = [](char j, char k) { return j <= k; };
        }
    }

    if (!lexi) {
        if (pattern[0] == '=' && pattern[1] == '=') {
            pattern.erase(0, 2);
        }
    }

    if (pattern[0] == '"' && pattern[pattern.size() - 1] == '"') {
        // remove "'s
        pattern.erase(0, 1);
        pattern.erase(pattern.size() - 1, 1);
    }

    return std::make_tuple(comparison, pattern, lexi);
}

bool match_int(const std::string &patternField, const std::string &tupleField) {
    try {
        size_t pos;
        int j = std::stoi(tupleField, &pos); // will throw if doesnt start with digit
        if (pos != tupleField.size()) return false; // non digit chars after int

        auto tuple = patternToIntComparison(patternField);
        auto comparison = std::get<0>(tuple);
        std::string val = std::get<1>(tuple);

        int k = std::stoi(val, &pos); //will throw if doesnt start with digit
        return comparison(j, k);
    }
    catch (std::invalid_argument &e) {
        return false;
    }
}

bool match_string(const std::string &patternField, const std::string &tupleField) {
    std::string a = tupleField;
    if (a[0] != '"' || a[a.size() - 1] != '"') return false;
    // remove "'s
    a.erase(0, 1);
    a.erase(a.size() - 1, 1);

    auto tuple = patternToStringComparison(patternField);
    auto comparison = std::get<0>(tuple);
    std::string b = std::get<1>(tuple);
    bool lexi = std::get<2>(tuple);

    auto aIt = a.begin(); // tuple
    auto bIt = b.begin(); // pattern with *s
    auto aItSave = a.end();
    auto bItSave = b.end();
    bool check = false;


    // check until first *
    for (; aIt != a.end() && bIt != b.end() && *bIt != '*'; ++aIt, ++bIt) {
        check = *aIt == *bIt || comparison(*aIt, *bIt);
        if (!check) return false;
        else if (*aIt != *bIt) return true; // lexicographic match
    }

    for (; aIt != a.end() && bIt != b.end(); ++bIt) {
        if (*bIt == '*') {
            // save points
            aItSave = aIt;
            bItSave = bIt;
        } else {
            check = *aIt == *bIt || comparison(*aIt, *bIt);
            if (check && *aIt != *bIt) return true;
            else {
                ++aIt;
                if (!check || !lexi && aIt != a.end() && bIt + 1 == b.end()) {
                    // if no * before then no match
                    if (aItSave == a.end() || bItSave == b.end()) return false;
                    // if there was a * before
                    bIt = bItSave; // reset pattern to save point
                    aIt = ++aItSave; // reset tuple to one char after save point, make new save point
                }
            }
        }
    }

    // remove trailing *s
    for (; bIt != b.end() && *bIt == '*'; ++bIt) {}

    if (bIt == b.end() && *(bIt - 1) == '*') return true; // * was last char in pattern

    // ik = 00 - tuple equally matched pattern          (==)
    //      01 - tuple had less chars than should match (<)
    //      10 - tuple had more chars than matched      (>)
    char i = aIt != a.end();
    char k = bIt != b.end();
    return comparison(i, k);
}

bool Pattern::match(const Tuple *t) const {
    std::string tuple = t->toString();
    // get rid of brackets first
    tuple.erase(tuple.begin());
    tuple.erase(tuple.size() - 1);

    std::vector<std::string> patternFields = split(pattern, ',');
    std::vector<std::string> tupleFields = split(tuple, ',');
    std::vector<std::string> fieldPair;
    if (patternFields.size() != tupleFields.size()) return false;

    for (int i = 0; i < patternFields.size(); ++i) {
        fieldPair = split(patternFields[i], ':');
        if (!strcasecmp(fieldPair[0].c_str(), "integer")) {
            if (!match_int(fieldPair[1], tupleFields[i])) return false;
        } else if (!strcasecmp(fieldPair[0].c_str(), "string")) {
            if (!match_string(fieldPair[1], tupleFields[i])) return false;
        }
    }
    return true;
}
