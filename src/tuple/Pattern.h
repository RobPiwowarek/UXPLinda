#ifndef UXPLINDA_PATTERN_H
#define UXPLINDA_PATTERN_H

#include <unordered_map>
#include "Tuple.h"

class Pattern {

public:
    Pattern();

    virtual ~Pattern();

    bool match(Tuple *tuple);

private:
    std::string pattern;
};


#endif //UXPLINDA_PATTERN_H
