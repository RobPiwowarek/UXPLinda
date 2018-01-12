#include <ctime>
#include <zconf.h>
#include <csignal>
#include "Client.h"

void Client::quit() {
    pid_t ppid = getppid();

    kill(ppid, SIGKILL);
}
