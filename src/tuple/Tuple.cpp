#include "Tuple.h"

Tuple::~Tuple() {

}

std::string Tuple::toString() const {
    return tuple;
}

Tuple::Tuple(Pattern *pattern) {

}

Tuple *Tuple::Empty() {
    return nullptr;
}

Tuple::Tuple(const std::string &tuple) : tuple(tuple) {

}

bool Tuple::operator==(const Tuple &other) const {
    return this->toString() == other.toString();
}
