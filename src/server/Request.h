#ifndef UXPLINDA_REQUEST_H
#define UXPLINDA_REQUEST_H

#include <csignal>

class Request {
public:

    enum class RequestType {
        OUTPUT,
        READ,
        INPUT
    };

    Request(RequestType type, pid_t clientID, char *data, int dataSize);

    virtual ~Request();

    int getDataSize() const;

    void setDataSize(int dataSize);

    pid_t getClientID() const;

    char *getData() const;

    void setData(char *data);

private:
    int dataSize;

    RequestType requestType;
public:
    RequestType getRequestType() const;

private:

    pid_t clientID;

    char *data;
};

#endif //UXPLINDA_REQUEST_H
