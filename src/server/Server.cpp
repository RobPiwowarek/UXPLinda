#include <iostream>
#include <wait.h>
#include "Server.h"

pthread_t Server::requestThread;
std::list<pid_t> Server::childrenPIDs;
bool Server::shouldStop = false;

Server::Server(std::list<char *> fileNames) {
    tupleSpace = new TupleSpace();
    initSignals();

    this->fileNames = fileNames;

    RESULT_PIPE_FDS = new int *[fileNames.size()];

    for (int i = 0; i < fileNames.size(); ++i) {
        RESULT_PIPE_FDS[i] = new int[2];
    }

    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);
}

Server::~Server() {
    delete tupleSpace;

    for (int i = 0; i < fileNames.size(); ++i){
        close(RESULT_PIPE_FDS[i][0]);
    }

    close(REQUEST_PIPE_FD);

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
}

int Server::run() {
    int tempFD[2];

    if (setupResultPipe(tempFD) == -1)
        return -1;

    setupAndExecClients(tempFD);

    // closing READ pipes for parent
    close(tempFD[1]);

    for (int i = 0; i < fileNames.size(); ++i) {
        close(RESULT_PIPE_FDS[i][0]);
    }

    if (pthread_create(&requestThread, nullptr, runRequestLoop, this)) {
        return -1; // error - brak zasobow?
    }

    return readingLoop();
}

int Server::setupResultPipe(int *tempFD) {
    if (pipe(tempFD) < 0) // tempFD[0] wejsciowy dla serwera, z tego odczytuje requesty od klientow
        return -1; // error -> brak wolnych deskryptorow
    REQUEST_PIPE_FD = tempFD[0];

    for (int i = 0; i < fileNames.size(); ++i) {
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
    for (auto file: fileNames) {
        pid_t pid = fork();

        if (pid == 0) {
            // tempFD[1] wyjsciowy dla requestow, dla klientow, ten sam dla kazdego klienta -> tutaj pisza requesty
            // otworz/zamknij odpowiedni pipe
            close(tempFD[0]);
            close(RESULT_PIPE_FDS[pipeIndex][1]);
            dup2(tempFD[1], Client::WriteFD);
            dup2(RESULT_PIPE_FDS[pipeIndex][0], Client::ReadFD);
            execve(file, nullptr, nullptr); // chyba lepiej uzyc execl
            // jesli exec sie nie udal zabij serwer
            std::cout << "could not exec " << file << ", aborting" << std::endl;
            pid_t ppid = getppid();
            kill(ppid, SIGINT);
        } else {
            childrenPIDs.push_back(pid);
        }

        ++pipeIndex;
    }

    return 0;
}

int Server::readingLoop() {

    while (!shouldStop) {
        ssize_t bytesRead;
        Request::RequestType type;
        pid_t clientPid;
        int datasize;
        char *data;

        bytesRead = read(REQUEST_PIPE_FD, &type, 4);
        if(bytesRead == 0) {
            pthread_cancel(Server::requestThread);
            return 0; // eof czyli dzieci zmarly, czas umrzec
        }
        if (bytesRead < 4) {
            std::cout << "Not enough data read to assemble type" << std::endl;
            return -666;
        }

        bytesRead = read(REQUEST_PIPE_FD, &clientPid, 4);
        if (bytesRead < 4) {
            std::cout << "Not enough data read to assemble clientPid" << std::endl;
            return -665;
        }

        bytesRead = read(REQUEST_PIPE_FD, &datasize, 4);
        if (bytesRead < 4) {
            std::cout << "Not enough data read to assemble dataSize" << std::endl;
            return -664;
        }

        data = new char[datasize];
        bytesRead = read(REQUEST_PIPE_FD, data, datasize);
        if (bytesRead < datasize) {
            std::cout << "Not enough data read to assemble data" << std::endl;
            return -663;
        }

        Request request(type, clientPid, data, datasize);
        pthread_mutex_lock(&mutex);
        processRequest(request);
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}

int Server::processRequest(Request request) {
    if (request.getRequestType() == Request::RequestType::OUTPUT) {
        Tuple tuple(request.getData());
        tupleSpace->output(tuple);

        if (!requests.empty()) {
            nothingNew = false;
            pthread_cond_broadcast(&cond); // sygnalizuj nowa krotke jesli sa czekajace w kolejce
        }
    } else if (request.getRequestType() == Request::RequestType::CANCEL) {
        // usun request ktory timeoutowal
        for (auto it = requests.begin(); it != requests.end(); ++it) {
            if (it->getClientID() == request.getClientID()) {
                requests.erase(it);
                break;
            }
        }
    } else {
        requests.push_back(request);
        nothingNew = false;
        pthread_cond_broadcast(&cond); // sygnalizuj nowy request w kolejce
    }

    return 0;
}

Tuple *Server::getTupleForRequest(Request request) {
    char *patternData = request.getData();

    Pattern p = Pattern(patternData);
    Tuple *t = nullptr;
    switch (request.getRequestType()) {
        case Request::RequestType::READ:
            t = tupleSpace->read(p);
            break;
        case Request::RequestType::INPUT:
            t = tupleSpace->input(p);
            break;
        default:
            break;
    }
    return t;
}

int Server::sendTuple(pid_t childPid, std::string tupleStr) {
    int fdIndex = 0;
    for (auto it : childrenPIDs) {
        if (it == childPid) break;
        ++fdIndex;
    }
    int childFD = RESULT_PIPE_FDS[fdIndex][1];
    write(childFD, tupleStr.c_str(), tupleStr.size() + 1); //null na koncu
}


int Server::requestLoop() {
    while (true) {
        pthread_mutex_lock(&mutex);
        if (nothingNew) {
            pthread_cond_wait(&cond, &mutex); // czeka na sygnal od readingLoop
        }
        // wyjdz jesli glowny watek dal sygnal a nie ma requestow
        if (requests.empty()) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        for (auto r = requests.begin(); r != requests.end(); ++r) {
            Tuple *t = getTupleForRequest(*r);

            if (t != nullptr) {
                pid_t childPid = r->getClientID();
                sendTuple(childPid, t->toString());
                delete t; // kopia robiona przez new
                requests.erase(r++);
                --r;
            }
        }
        nothingNew = true;
        pthread_mutex_unlock(&mutex);
    }
}

void *Server::runRequestLoop(void *s) {
    auto server = (Server *) s;
    server->requestLoop();

    return nullptr;
}

void signalHandler(int sig) {
    if (sig == SIGINT || sig == SIGKILL) {
        for (auto child: Server::childrenPIDs) {
            kill(child, SIGKILL);
        }

        pthread_cancel(Server::requestThread);

        for (auto child: Server::childrenPIDs) {
            int status = -1;
            waitpid(child, &status, WEXITED);
        }

        Server::shouldStop = true;

        exit(0); // lazy as fuck other than that i can set a flag for while loop.
    }
}

void Server::initSignals() {
    sigact.sa_handler = signalHandler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGINT, &sigact, (struct sigaction *) nullptr);
    sigaction(SIGKILL, &sigact, (struct sigaction *) nullptr);
}