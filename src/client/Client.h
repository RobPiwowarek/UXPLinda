#ifndef UXPLINDA_CLIENT_H
#define UXPLINDA_CLIENT_H

#include <string>
#include <semaphore.h>
#include <chrono>
#include "../server/Request.h"
#include "../tuple/Tuple.h"
#include "../tuple/Pattern.h"

class Client {
    pid_t pid;
    std::string getTupleFromServer(Request::RequestType requestType, const std::string &pattern, timeval *timeout);
    Request::RequestType lastRequest;

    void handleTimeout();

    std::string handleSuccess(const std::string &patter, timeval previousTimeout, timeval startTime);

    char* createRequest(Request::RequestType requestType, const std::string &message);

    std::string receiveTuple();
    void removeCanceledTupleFromPipe();
    void setNonBlockingRead();
    void setBlockingRead();
public:
    static const int ReadFD = 1023;
    static const int WriteFD = 1022;

    Client();

    ~Client();

    std::string input(const std::string &inputTuple, timeval *timeout);

    std::string read(const std::string &pattern, timeval *timeout);

    bool output(std::string tuple);

    void quit();

    int getRequestSize(const std::string &message);
};


#endif //UXPLINDA_CLIENT_H
