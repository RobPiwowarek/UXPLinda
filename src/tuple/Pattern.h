#ifndef UXPLINDA_PATTERN_H
#define UXPLINDA_PATTERN_H

// dont judge me for this
#define PATTERN_REGEX R"((((String:\s*(((>|<|>=|<=|==)?"[^"]*")|\*))|(Integer:\s*(((>|<|>=|<=|==)?(-?\d+))|\*)))(,\s*((String:\s*(((>|<|>=|<=|==)?"[^"]*")|\*))|(Integer:\s*(((>|<|>=|<=|==)?(-?\d+))|\*))))*))"

#include <unordered_map>

class Tuple;

class Pattern {

public:
    Pattern();

    Pattern(const std::string &pattern);

    virtual ~Pattern();

    bool match(const Tuple *tuple) const;

private:
    std::string pattern;
};


#endif //UXPLINDA_PATTERN_H
