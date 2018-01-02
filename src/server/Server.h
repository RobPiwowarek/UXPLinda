#ifndef UXPLINDA_SERVER_H
#define UXPLINDA_SERVER_H

#include "../client/Client.h"
#include "../tuple/TupleSpace.h"
#include "Request.h"
#include <deque>
#include <list>
#include <unistd.h>

class Server {

public:
    explicit Server(std::list<char*>);

    int run();

    virtual ~Server();

private:
    TupleSpace *tupleSpace;

    std::list<pid_t> childrenPIDs;
    std::list<char*> fileNames;

    int REQUEST_PIPE_FD;
    int **RESULT_PIPE_FDS; // fixme: moze dla pewnosci lepiej mapa PID -> int laczaca dziecko z odpowiednim pipem?
};


#endif //UXPLINDA_SERVER_H
