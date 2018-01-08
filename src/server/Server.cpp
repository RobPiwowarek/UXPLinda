#include "Server.h"

Server::Server(std::list<char*> fileNames) {
    tupleSpace = new TupleSpace();

    this->fileNames = fileNames;

    RESULT_PIPE_FDS = new int*[fileNames.size()];

    for (int i = 0; i < fileNames.size(); ++i){
        RESULT_PIPE_FDS[i] = new int[2];
    }
}

Server::~Server() {
    delete tupleSpace;
}

int Server::run() {
    int tempFD[2];

    if (setupResultPipe(tempFD) == -1)
        return -1;

    setupAndExecClients(tempFD);

    // closing READ pipes for parent
    close(tempFD[1]);

    for(int i = 0; i < fileNames.size(); ++i){
        close(RESULT_PIPE_FDS[i][0]);
    }

    //todo:
    sem = sem_open(SNAME, O_CREAT, 0644, 3);

    return readingLoop();
}

int Server::setupResultPipe(int *tempFD) {
    if (pipe(tempFD) < 0) // tempFD[0] wejsciowy dla serwera, z tego odczytuje requesty od klientow
        return -1; // error -> brak wolnych deskryptorow
    REQUEST_PIPE_FD = tempFD[0];

    for (int i = 0; i < fileNames.size(); ++i){
        int clientFD[2];

        if (pipe(clientFD) < 0)
            return -1; // error -> brak wolnych deskryptorow

        RESULT_PIPE_FDS[i][0] = clientFD[0];
        RESULT_PIPE_FDS[i][1] = clientFD[1];
    }

    return 0;
}

int Server::setupAndExecClients(int *tempFD) {
    int pipeIndex = 0;
    for (auto file: fileNames){
        pid_t pid = fork();

        if (pid == 0){
            // tempFD[1] wyjsciowy dla requestow, dla klientow, ten sam dla kazdego klienta -> tutaj pisza requesty
            // otworz/zamknij odpowiedni pipe
            close(tempFD[0]);
            close(RESULT_PIPE_FDS[pipeIndex][1]);
            dup2(tempFD[1], Client::WriteFD);
            dup2(RESULT_PIPE_FDS[pipeIndex][0], Client::ReadFD);
            execve(file, nullptr, nullptr); // chyba lepiej uzyc execl
        }
        else {
            childrenPIDs.push_back(pid);
        }

        ++pipeIndex;
    }

    return 0;
}

int Server::readingLoop() {

    while (true) { // todo: obsluga sygnalu czy cos ktora by to zamknela
        sem_wait(sem);

        ssize_t bytesRead;
        Request::RequestType type;
        pid_t clientPid;
        int datasize;
        char *data;

        // fixme: mam nadzieje ze to nie popsuje enuma?
        bytesRead = read(REQUEST_PIPE_FD, &type, 1);
        if (bytesRead < 1){
            // todo: error
            break;
        }

        bytesRead = read(REQUEST_PIPE_FD, &clientPid, 4);
        if (bytesRead < 4) {
            // todo: error
            break;
        }

        bytesRead = read(REQUEST_PIPE_FD, &datasize, 4);
        if (bytesRead < 4) {
            // todo: error
            break;
        }

        data = new char[datasize];

        bytesRead = read(REQUEST_PIPE_FD, data, static_cast<size_t>(datasize));
        if (bytesRead < datasize){
            // todo: error
            break;
        }

        Request request(type, clientPid, data, datasize);
        processRequest(request);
    }

    return 0;
}

int Server::processRequest(Request request) {
    if (request.getRequestType() == Request::RequestType::OUTPUT){
        Tuple tuple(request.getData());
        tupleSpace->output(tuple);
    }
    else {

    }

    return 0;
}
