#ifndef UXPLINDA_CMDPARSER_H
#define UXPLINDA_CMDPARSER_H

#include <list>

class CMDParser {

public:

    // wez argc, argv
    // zwroc liste nazw programow
    static std::list<char*> parse(int argc, char** argv);
};


#endif //UXPLINDA_CMDPARSER_H
