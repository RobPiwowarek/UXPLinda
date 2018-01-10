#include "CMDParser.h"

std::list<char*> CMDParser::parse(int argc, char **argv) {
    std::list<char*> programs;

    for (int i = 1; i < argc; ++i){
        programs.push_back(argv[i]);
    }

    return programs;
}
