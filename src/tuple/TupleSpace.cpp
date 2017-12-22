#include "TupleSpace.h"

TupleSpace::TupleSpace() {}

TupleSpace::~TupleSpace() {

}

Tuple *TupleSpace::input(const Pattern &pattern, unsigned int timeout) {
    return nullptr;
}

Tuple *TupleSpace::read(const Pattern &pattern, unsigned int timeout) {
    return nullptr;
}

bool TupleSpace::output(const Tuple &tuple) {
    return false;
}
