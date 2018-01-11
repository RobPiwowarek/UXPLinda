#include <ctime>
#include <string>     // std::string, std::to_string
#include <zconf.h>
#include <signal.h>
#include "Client.h"

//TODO change Tuple to string

Client::Client() {
    pid = getpid();
    sem = sem_open(SNAME, O_CREAT, 0644, 3);
}

Client::~Client() {

}


Tuple Client::input(const std::string &tupleString, timeval *timeout) {
    return getTupleFromServer(Request::RequestType::INPUT, tupleString, timeout);
}

Tuple Client::read(const std::string &tupleString, timeval *timeout) {
    return getTupleFromServer(Request::RequestType::READ, tupleString, timeout);
}

char *getWritable(const std::string &str) {
    auto *writable = new char[str.size() + 1];
    std::copy(str.begin(), str.end(), writable);
    writable[str.size()] = '\0';
    return writable;
}

void Client::handleTimeout() {
    auto *cancellation = new Request(Request::RequestType::CANCEL, pid, nullptr, 0);
    write(WriteFD, cancellation, sizeof(cancellation));
    sem_post(sem);
}

Tuple Client::handleSuccess() {
    char *data;
    int bytesRead = ::read(ReadFD, &data, 4);
    if (bytesRead < 4) {
        return Tuple("");
    }
    int dataSize = std::stoi(data);
    data = new char[dataSize];
    bytesRead = ::read(ReadFD, data, static_cast<int>(dataSize));
    if (bytesRead < dataSize) {
        return Tuple("");
    }
    return Tuple(std::string(data));
    //TODO rozmiar, czy wiadomosc taka sama jak do serwera czy już z mniejszym nagłówkiem
}

Tuple Client::getTupleFromServer(Request::RequestType requestType, const std::string &data, timeval *timeout) {
    std::string str;
    str += std::to_string(static_cast<int>(requestType));
    str += std::to_string(pid);
    str += std::to_string(data.size());
    str += data;

    write(WriteFD, str.c_str(), str.size()+1);//?+1?
    sem_post(sem);

    fd_set set;
    FD_ZERO(&set);
    FD_SET(ReadFD, &set);

    timeval timeout_select = *timeout; //select zeroes timeval!
    int value = select(ReadFD + 1, &set, nullptr, nullptr, &timeout_select); //ReadFD+1 couse select needs so

    if (value == -1) {//error
        return Tuple("");
    } else if (value == 0) {//timeOut
        handleTimeout();
    } else {//success
        handleSuccess();
    }
}

bool Client::output(const Tuple &tuple) {
    std::string str;
    str += std::to_string(static_cast<int>(Request::RequestType::OUTPUT));
    str += std::to_string(pid);
    str += std::to_string(tuple.toString().size());
    str += tuple.toString();
    write(WriteFD, str.c_str(), str.size()+1);
}

void Client::quit() {
    pid_t ppid = getppid();

    kill(ppid, SIGKILL);
}
