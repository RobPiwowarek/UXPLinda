#ifndef UXPLINDA_TUPLESPACE_H
#define UXPLINDA_TUPLESPACE_H

#include "Tuple.h"
#include "Pattern.h"
#include <unordered_set>

class TupleSpace {

public:
    TupleSpace();

    virtual ~TupleSpace();

    // dodanie do przestrzeni podana krotki
    bool output(const Tuple &tuple);

    Tuple *read(const Pattern &pattern) const;

    Tuple *input(const Pattern &pattern);

private:
    std::unordered_set<Tuple> tuples;

};


#endif //UXPLINDA_TUPLESPACE_H
