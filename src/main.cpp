#include <iostream>
#include <signal.h>
#include <string.h>

#include "config.h"
#include "Net/TCPConnection.h"
#include "Net/TCPServer.h"

using namespace std;

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

    hlt::TCPServer clientServer(string(CLIENT_PORT));
    clientServer.start();
    hlt::TCPServer spServer(string(SP_PORT));
    spServer.start();

    while (1) {
        string in;
        cin >> in;
        // Possible UI here
    }

    return 0;
}
