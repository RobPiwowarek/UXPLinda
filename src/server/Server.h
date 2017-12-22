#ifndef UXPLINDA_SERVER_H
#define UXPLINDA_SERVER_H

#include "../client/Client.h"
#include "../tuple/TupleSpace.h"
#include "Request.h"
#include <deque>

class Server {

public:
    Server();

    virtual ~Server();

private:
    TupleSpace *tupleSpace;

    std::deque<Request> requests;
};


#endif //UXPLINDA_SERVER_H
