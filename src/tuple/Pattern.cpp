#include <vector>
#include <sstream>
#include <cstring>
#include <functional>
#include <regex>
#include "Pattern.h"
#include "Tuple.h"

Pattern::Pattern() {}

Pattern::Pattern(const std::string &patter) : pattern(patter){}

Pattern::~Pattern() {

}

std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        if (token[0] == ' ') token.erase(0,1);
        tokens.push_back(token);
    }
    return tokens;
}

bool match_int(std::string patternField, const std::string &tupleField) {
    try {
        size_t pos;
        int j = std::stoi(tupleField, &pos); // will throw if doesnt start with digit
        if (pos != tupleField.size()) return false; // trash characters after int in tuple
        std::function<bool(int, int)> comparison;

        comparison = [](int j, int k) { return j == k; };
        if (patternField[0] == '=' && patternField[1] == '=') {
            patternField.erase(0, 2);
        } else if (patternField[0] == '>') {
            patternField.erase(0);
            comparison = [](int j, int k) { return j > k; };
            if (patternField[1] == '=') {
                patternField.erase(0);
                comparison = [](int j, int k) { return j >= k; };
            }
        } else if (patternField[0] == '<') {
            patternField.erase(0);
            comparison = [](int j, int k) { return j < k; };
            if (patternField[1] == '=') {
                patternField.erase(0);
                comparison = [](int j, int k) { return j <= k; };
            }
        } else if (patternField[0] == '*') {
            comparison = [](int j, int k) { return true; };
        } else if (patternField[0] < '0') return false; //invalid pattern

        int k = std::stoi(patternField, &pos); //will throw if doesnt start with digit
        if (pos != patternField.size()) return false;
        if (!comparison(j, k)) return false; //match failed
    }
    catch (std::invalid_argument &e) {
        return false;
    }
}

bool match_string(std::string patternField, std::string tupleField) {
    std::string &a = tupleField;
    if (a[0] != '"' || a[a.size() - 1 != '"']) return false;
    // remove "'s
    a.erase(0);
    a.erase(a.size() - 1);

    std::string &b = patternField;

    std::function<bool(std::string, std::string)> comparison;

    //comparison = [](char j, char k) { return j == k; };
    if (b[0] == '=' && b[1] == '=') {
        b.erase(0, 2);
    }
    if (b[0] == b[b.size() - 1] == '"') {
        // remove "'s
        b.erase(0);
        b.erase(b.size() - 1);

        // TODO - regex for equal comparison???
    }

    // assuming no * - maybe regex too????????
    if (b[0] == '>') {
        b.erase(0);
        comparison = [](std::string j, std::string k) { return j > k; };
        if (b[1] == '=') {
            b.erase(0);
            comparison = [](std::string j, std::string k) { return j >= k; };
        }
    } else if (b[0] == '<') {
        b.erase(0);
        comparison = [](std::string j, std::string k) { return j < k; };
        if (b[1] == '=') {
            b.erase(0);
            comparison = [](std::string j, std::string k) { return j <= k; };
        }
    } else if (b == "*") {
        return true;
    } else return false; // invalid pattern
    if (b[0] != '"' || b[b.size() - 1 != '"']) return false;

    // remove "'s
    b.erase(0);
    b.erase(b.size() - 1);

    if (!comparison(a, b)) return false;
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
            return match_int(fieldPair[1], tupleFields[i]);
        } else if (!strcasecmp(fieldPair[0].c_str(), "string")) {
            return match_string(fieldPair[1], tupleFields[i]);
        }
    }
    return true;
}

