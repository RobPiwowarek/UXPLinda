#include <iostream>
#include "Tuple.h"

bool isTuple(const std::string& s) {
    return std::regex_match(s, std::regex(TUPLE_REGEX));
}

Tuple::~Tuple() {

}

std::string Tuple::toString() const {
    return tuple;
}

Tuple *Tuple::Empty() {
    return nullptr;
}

Tuple::Tuple(const std::string &tuple) : tuple(tuple) {
    if(!isTuple(tuple)) {
        throw std::invalid_argument("String is not a valid tuple");
    }
}

bool Tuple::operator==(const Tuple &other) const {
    return this->toString() == other.toString();
}
