#ifndef UXPLINDA_SERVER_H
#define UXPLINDA_SERVER_H

#include "../client/Client.h"
#include "../tuple/TupleSpace.h"
#include "Request.h"
#include <list>
#include <vector>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#define SNAME "/mysem"

class Server {
    sem_t *sem;// = sem_open(SNAME, O_CREAT, 0644, 3);
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_t requestThread;

public:
    explicit Server(std::list<char*>);

    int run();

    virtual ~Server();

private:
    TupleSpace *tupleSpace;

    int setupResultPipe(int []);
    int setupAndExecClients(int []);

    int readingLoop();
    int requestLoop();
    static void* runRequestLoop(void*);

    int processRequest(Request request);

    Tuple* getTupleForRequest(Request request);
    int sendTuple(pid_t childPid, std::string tupleStr);

    bool nothingNew = true;
    std::list<Request> requests;
    std::vector<pid_t> childrenPIDs;
    std::list<char*> fileNames;

    int REQUEST_PIPE_FD;
    int **RESULT_PIPE_FDS; // fixme: moze dla pewnosci lepiej mapa PID -> int laczaca dziecko z odpowiednim pipem?

};

#endif //UXPLINDA_SERVER_H
