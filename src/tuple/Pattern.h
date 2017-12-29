#ifndef UXPLINDA_PATTERN_H
#define UXPLINDA_PATTERN_H

#include <unordered_map>
// i nie myslcie nawet o przesunieciu pattern przed tuple w cmakelists
//#include "Tuple.h"

class Tuple;
class Pattern {

public:
    Pattern();

    virtual ~Pattern();

    bool match(Tuple *tuple);

private:
    std::string pattern;
};


#endif //UXPLINDA_PATTERN_H
