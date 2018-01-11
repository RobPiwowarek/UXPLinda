#include <iostream>
#include "src/server/Server.h"
#include "src/server/CMDParser.h"
#include <unistd.h>

int main(int argc, char** argv) {
    CMDParser parser;

    std::list<pid_t> childrenPIDs;

    std::list<char*> fileNames = parser.parse(argc, argv);

    Server server(fileNames);

    server.run();

    Client c;
    timeval timeout;
    timeout.tv_sec = 3;
    c.input("String:*", &timeout);

}