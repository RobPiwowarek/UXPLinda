#ifndef UXPLINDA_TUPLE_H
#define UXPLINDA_TUPLE_H

#define TUPLE_REGEX R"(\((((("[^"]*")|(-?(\d)+)))(,\s*(("[^"]*")|(-?(\d)+)))*)\))"

#include <unordered_map>
#include <regex>

class Tuple {
public:

    explicit Tuple(const std::string &tuple);

    virtual ~Tuple();

    bool operator==(const Tuple& other) const;

    std::string toString() const;

    static Tuple *Empty();

private:
    std::string tuple;
};


namespace std {
    template<>
    struct hash<Tuple> {
        size_t operator()(Tuple const &tuple) const noexcept {
            std::hash<std::string> hasher;

            std::string str = tuple.toString();

            return hasher(str);
        }
    };
}
#endif //UXPLINDA_TUPLE_H
