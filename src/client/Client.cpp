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
    pid = getpid();
}

Client::~Client() {
    removeCanceledTupleFromPipe();
    close(ReadFD);
    close(WriteFD);
}

std::string Client::input(const std::string &pattern, timeval *timeout) {
    return getTupleFromServer(Request::RequestType::INPUT, pattern, timeout);
}

std::string Client::read(const std::string &pattern, timeval *timeout) {
    return getTupleFromServer(Request::RequestType::READ, pattern, timeout);
}


void Client::handleTimeout() {
    char *request = createRequest(Request::RequestType::CANCEL, "");
    write(WriteFD, request, getRequestSize(""));
    removeCanceledTupleFromPipe();
}

std::string Client::getTupleFromServer(Request::RequestType requestType, const std::string &patternString, timeval *timeout) {
    try {
        Pattern pattern = Pattern(patternString);
    } catch (const std::invalid_argument &error) {
        return "Incorrect pattern";
    }
    removeCanceledTupleFromPipe();

    char *str = createRequest(requestType, patternString);
    write(WriteFD, str, getRequestSize(patternString));
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
        std::cout << error.what() << std::endl;
        return false;
    }
    char *str = createRequest(Request::RequestType::OUTPUT, tuple);
    int success = write(WriteFD, str, getRequestSize(tuple));
    delete str;
    return success;
}

void Client::quit() {
    pid_t ppid = getppid();

    kill(ppid, SIGINT);
}


int Client::getRequestSize(const std::string& message) {
    return 12 + message.size() + 1;
}

char *Client::createRequest(Request::RequestType requestType, const std::string &message = "") {
    int messageLength = message.size() + 1;
    int requestSize = getRequestSize(message);
    auto *request = new char[requestSize];
    memcpy(request, &requestType, 4);
    memcpy(request + 4, &pid, 4);
    memcpy(request + 8, &messageLength, 4);
    memcpy(request + 12, message.c_str(), messageLength);
    return request;
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
    if (tupleString.size()>0) {//send it back
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

