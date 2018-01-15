#include <ctime>
#include <string>     // std::string, std::to_string
//#include <zconf.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <climits>
#include <fcntl.h>
#include <sys/time.h>
#include "Client.h"

Client::Client() {
    lastCanceledRequest = Request::RequestType::INPUT;
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


void Client::handleTimeout(Request::RequestType type) {
    lastCanceledRequest = type;
    char *request = createRequest(Request::RequestType::CANCEL, "");
    write(WriteFD, request, getRequestSize(""));
    removeCanceledTupleFromPipe();
    delete request;
}

timeval subTimevals(const timeval &tv0, const timeval &tv1) {
    timeval result;
    result.tv_sec = tv0.tv_sec - tv1.tv_sec;
    result.tv_usec = tv0.tv_usec - tv1.tv_usec;
    return result;
}

std::string Client::handleSuccess(const std::string &patterString, timeval previousTimeout, timeval startTime) {
    std::string received = receiveTuple();
    Pattern pattern(patterString);
    Tuple receivedTuple(received);
    if (pattern.match(&receivedTuple)) {
        return received;
    }
    if (lastCanceledRequest == Request::RequestType::INPUT) {
        output(received);
    }
    timeval time;
    gettimeofday(&time, 0);
    timeval timeTaken = subTimevals(time, startTime);
    timeval newTimeout = subTimevals(previousTimeout, timeTaken);
    return input(patterString, &newTimeout);
}

std::string Client::getTupleFromServer(Request::RequestType requestType, const std::string &patternString, timeval *timeout) {
    try {
        Pattern pattern = Pattern(patternString);
    } catch (const std::invalid_argument &error) {
        return "Incorrect pattern";
    }

    char *str = createRequest(requestType, patternString);
    removeCanceledTupleFromPipe();

    timeval requestStartTime;
    gettimeofday(&requestStartTime, 0);
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
        handleTimeout(requestType);
        return "timeout";
    } else {//success
        return handleSuccess(patternString, *timeout, requestStartTime);
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


int Client::getRequestSize(const std::string &message) {
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
    if (bytesRead < 1) {
        return "";
    }
    int length = strlen(data);
    std::string tuple(data, length);
    delete data;
    return tuple;
}

void Client::removeCanceledTupleFromPipe() {
    setNonBlockingRead();
    std::string tupleString = receiveTuple();
    setBlockingRead();
    if (tupleString.size() > 2 && lastCanceledRequest == Request::RequestType::INPUT) {//send it
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

