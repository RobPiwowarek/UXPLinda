#ifndef UXPLINDA_SERVER_H
#define UXPLINDA_SERVER_H

#include "../client/Client.h"
#include "../tuple/TupleSpace.h"
#include "Request.h"
#include <list>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#define SNAME "/mysem"

class Server {
    sem_t *sem;// = sem_open(SNAME, O_CREAT, 0644, 3);

public:
    explicit Server(std::list<char*>);

    int run();

    virtual ~Server();

private:
    TupleSpace *tupleSpace;

    int setupResultPipe(int []);
    int setupAndExecClients(int []);

    int readingLoop();

    int processRequest(Request request);

    std::list<Request> requests;
    std::list<pid_t> childrenPIDs;
    std::list<char*> fileNames;

    int REQUEST_PIPE_FD;
    int **RESULT_PIPE_FDS; // fixme: moze dla pewnosci lepiej mapa PID -> int laczaca dziecko z odpowiednim pipem?
};

#endif //UXPLINDA_SERVER_H
