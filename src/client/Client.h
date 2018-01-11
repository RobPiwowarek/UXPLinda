#ifndef UXPLINDA_CLIENT_H
#define UXPLINDA_CLIENT_H

#include <server/Request.h>
#include <server/Server.h>
#include <semaphore.h>
#include <tuple/Tuple.h>

class Client {
    pid_t pid;
    sem_t *sem;// = sem_open(SNAME, O_CREAT, 0644, 3);
    Tuple getTupleFromServer(Request::RequestType requestType, const std::string &inputTuple, timeval *timeout);

    void handleTimeout();

    Tuple handleSuccess();

public:
    static const int ReadFD = 1023;
    static const int WriteFD = 1022;

    Client();

    ~Client();

    Tuple input(const std::string &inputTuple, timeval *timeout);

    Tuple read(const std::string &inputTuple, timeval *timeout);

    bool output(const Tuple &tuple);

    void quit();
};


#endif //UXPLINDA_CLIENT_H
