#ifndef __HLT_TCPSERVER_SERVER_H__
#define __HLT_TCPSERVER_SERVER_H__

#include <exception>
#include <memory>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <thread>
#include <vector>

#include "EventEmitter.hpp"
#include "TCPConnection.h"

using std::shared_ptr;
using std::string;
using std::vector;

#define BACKLOG 10

namespace hlt {

class TCPServer {
public:
    struct EventArgument {
        TCPConnection::Ptr connection;
        string message;
        ~EventArgument(){}
    };

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

    /**
     * Returns event emitter of server so that users of server class
     * can add handler for events 
     * 
     * @return Event emitter object
     */
    EventEmitter<EventArgument>& getEventEmitter();
private:
    struct addrinfo* createSocket(struct addrinfo* serverInfo);

    bool running;
    string port;
    struct addrinfo hints;

    int socketFd;

    std::thread* servingThread;
    std::thread* connectionCleaner;

    vector<TCPConnection::Ptr> connections;
    EventEmitter<EventArgument> eventEmitter;
};

class TCPServerException : public std::exception {
public:
    TCPServerException(std::string message) : message(message) {}
    std::string what() { return message; }
private:
    std::string message;
};

}

#endif
