#ifndef __HLT_TCPSERVER_SERVER_H__
#define __HLT_TCPSERVER_SERVER_H__

#include <memory>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <thread>
#include <vector>

#include "TCPConnection.h"

using std::shared_ptr;
using std::string;
using std::vector;

#define BACKLOG 10

namespace hlt {

class TCPServer {
public:
    /**
     *  Constructor, sets configurations for socket
     */
    TCPServer(string port);
    ~TCPServer();

    /**
     * Starts server on given port
     */
    void start();
    /**
     * Stops server if running
     */
    void stop();

private:
    bool running;
    string port;
    struct addrinfo hints;

    int socketFd;

    std::thread* servingThread;
    std::thread* connectionCleaner;

    vector<shared_ptr<TCPConnection>> connections;
};

}

#endif
