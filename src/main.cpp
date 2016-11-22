#include <iostream>
#include <signal.h>
#include <string.h>

#include "Net/TCPServer.h"

using namespace std;

#define PORT "8081"  // the port users will be connecting to

#define BACKLOG 10     // how many pending connections queue will hold

static void signalHandler(int signum) {
    
}

int main(int argc, char** argv)
{
    struct addrinfo hints, *servinfo, *p;
    int rv;

    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGPIPE, &sa, NULL) == -1) {
        cout << "Couldn't set signal handler. Exiting..." << endl;
        return 1;
    }

    hlt::TCPServer server(string(PORT));
    server.start();

    return 0;
}
