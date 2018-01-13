#include <ctime>
#include <string>     // std::string, std::to_string
//#include <zconf.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <climits>
#include <fcntl.h>
#include "Client.h"

Client::Client() {
    lastRequest=Request::RequestType::INPUT;
    pid = getpid();
}

Client::~Client() {
    removeCanceledTupleFromPipe();
    close(ReadFD);
    close(WriteFD);
}

std::string Client::input(const std::string &pattern, timeval *timeout) {
    lastRequest=Request::RequestType::INPUT;
    return getTupleFromServer(Request::RequestType::INPUT, pattern, timeout);
}

std::string Client::read(const std::string &pattern, timeval *timeout) {
    lastRequest=Request::RequestType::READ;
    return getTupleFromServer(Request::RequestType::READ, pattern, timeout);
}


void Client::handleTimeout() {
    char *request = createRequest(Request::RequestType::CANCEL, "");
    write(WriteFD, request, strlen(request));
    removeCanceledTupleFromPipe();
}

std::string Client::getTupleFromServer(Request::RequestType requestType, const std::string &patternString, timeval *timeout) {
    try {
        Pattern pattern = Pattern(patternString);
    } catch (const std::invalid_argument &error) {
        return "zly patternString";
    }
    removeCanceledTupleFromPipe();
    int patternSize = patternString.size() + 1;

    int msgSize = 4 + 4 + 4 + patternSize;
    char *str = new char[msgSize];
    memcpy(str, &requestType, 4);
    memcpy(str + 4, &pid, 4);
    memcpy(str + 8, &patternSize, 4);
    memcpy(str + 12, patternString.c_str(), patternSize);
    write(WriteFD, str, msgSize);
    delete str;

    fd_set set;
    FD_ZERO(&set);
    FD_SET(ReadFD, &set);

    timeval timeout_select = *timeout;
    int value = select(ReadFD + 1, &set, nullptr, nullptr, &timeout_select); //ReadFD+1 cause select needs so

    if (value == -1) {//error
        return "error";
    } else if (value == 0) {//timeOut
        handleTimeout();
        return "timeout";
    } else {//success
        return receiveTuple();
    }
}

bool Client::output(std::string tuple) {
    try {
        Tuple temp = Tuple(tuple);
    } catch (const std::invalid_argument &error) {
        std::cout << error.what();
        return false;
    }
    int tupleSize = tuple.size() + 1;
    Request::RequestType type = Request::RequestType::OUTPUT;

    int msgSize = 4 + 4 + 4 + tupleSize;
    char *str = new char[msgSize];
    memcpy(str, &type, 4);
    memcpy(str + 4, &pid, 4);
    memcpy(str + 8, &tupleSize, 4);
    memcpy(str + 12, tuple.c_str(), tupleSize);
    int success = write(WriteFD, str, msgSize);
    delete str;
    return success;
}

void Client::quit() {
    pid_t ppid = getppid();

    kill(ppid, SIGINT);
}

char *Client::createRequest(Request::RequestType requestType, const std::string &message = "") {
    int messageLength = message.size();
    int requestSize = 4 + 4 + 4 + messageLength + 1;
    char *request = new char[requestSize];
    memcpy(request, &requestType, 4);
    memcpy(request + 4, &pid, 4);
    memcpy(request + 8, &messageLength, 4);
    if (messageLength > 0) {
        memcpy(request + 12, message.c_str(), requestSize);
    }
}

std::string Client::receiveTuple() {
    char *data = new char[PIPE_BUF];
    int bytesRead = ::read(ReadFD, data, PIPE_BUF);
    int length = strlen(data);
    std::string tuple(data, length);
    return tuple;
}

void Client::removeCanceledTupleFromPipe() {
    setNonBlockingRead();
    std::string tupleString = receiveTuple();
    setBlockingRead();
    if (tupleString.size()>2 && lastRequest==Request::RequestType::INPUT) {//send it back
        output(tupleString);
    }
}

void ::Client::setBlockingRead() {
    int flags = fcntl(ReadFD, F_GETFL, 0);
    fcntl(ReadFD, F_SETFL, flags & ~O_NONBLOCK);
}

void ::Client::setNonBlockingRead() {
    int flags = fcntl(ReadFD, F_GETFL, 0);
    fcntl(ReadFD, F_SETFL, flags | O_NONBLOCK);
}

