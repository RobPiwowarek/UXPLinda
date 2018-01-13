#include "TupleSpace.h"

TupleSpace::TupleSpace() {}

TupleSpace::~TupleSpace() {

}

Tuple *TupleSpace::input(const Pattern &pattern) {
    Tuple *t = nullptr;
    for (auto it = tuples.begin(); it != tuples.end(); ++it) {
        if (pattern.match(&(*it))) {
            t = new Tuple(it->toString());
            tuples.erase(it);
            break;
        }
    }
    return t;
}

Tuple *TupleSpace::read(const Pattern &pattern) const {
    Tuple *t = nullptr;
    for (auto it = tuples.begin(); it != tuples.end(); ++it) {
        if (pattern.match(&(*it))) {
            t = new Tuple(it->toString());
            break;
        }
    }
    return t;
}

bool TupleSpace::output(Tuple tuple) {
    tuples.push_back(tuple);
    return true;
}
