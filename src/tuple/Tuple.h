#ifndef UXPLINDA_TUPLE_H
#define UXPLINDA_TUPLE_H

#include <unordered_map>
#include "Pattern.h"

class Tuple {
public:
    explicit Tuple(Pattern *pattern);

    virtual ~Tuple();

    std::string toString() const;

    static Tuple * Empty();

private:
    std::string tuple;
};


// TODO: trzeba zrobic hasha dla wlasnej klasy dla unordered_set'a, przyklad przekopiowany ze stacka ale z jakiegos powodu nie dziala

struct hash<Tuple>
{
    size_t operator()(Tuple const & tuple) const noexcept
    {
        std::hash<std::string> hasher;

        std::string str = tuple.toString();

        return hasher(str);
    }
};

#endif //UXPLINDA_TUPLE_H
