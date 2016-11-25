#include <iostream>
#include <signal.h>
#include <string.h>

#include "config.h"
#include "Net/TCPConnection.h"
#include "Net/TCPServer.h"

using namespace std;

static void signalHandler(int signum) {
    // TODO Handle signals
}

int main(int argc, char** argv)
{
    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGPIPE, &sa, NULL) == -1) {
        cout << "Couldn't set signal handler. Exiting..." << endl;
        return 1;
    }

    hlt::TCPServer clientServer(string(CLIENT_PORT));
    clientServer.getEventEmitter().on("connection", 
            [](const hlt::TCPServer::EventArgument& arg) {
                cout << "New connection to client server!" << endl;
            });
    clientServer.start();
    hlt::TCPServer spServer(string(SP_PORT));
    spServer.getEventEmitter().on("connection", 
            [](const hlt::TCPServer::EventArgument& arg) {
                cout << "New connection to service provider server!" << endl;
            });
    spServer.start();

    while (1) {
        string in;
        cin >> in;
        // Possible UI here
    }

    return 0;
}
