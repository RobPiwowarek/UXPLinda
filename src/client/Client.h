#ifndef UXPLINDA_CLIENT_H
#define UXPLINDA_CLIENT_H

#include <string>
#include <semaphore.h>
#include "../server/Request.h"
#include "../tuple/Tuple.h"
#include "../tuple/Pattern.h"

class Client {
    pid_t pid;
    std::string getTupleFromServer(Request::RequestType requestType, const std::string &pattern, timeval *timeout);

    void handleTimeout();

    std::string handleSuccess();

public:
    static const int ReadFD = 1023;
    static const int WriteFD = 1022;

    Client();

    ~Client();

    std::string input(const std::string &inputTuple, timeval *timeout);

    std::string read(const std::string &pattern, timeval *timeout);

    bool output(std::string tuple);

    void quit();
};


#endif //UXPLINDA_CLIENT_H
