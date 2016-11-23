#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "TCPServer.h"

#include <iostream>

using std::cout;
using std::endl;

namespace hlt {

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

TCPServer::TCPServer(string port) : port(port) {
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
}

TCPServer::~TCPServer() {
    // Intentionally empty
}

void TCPServer::start() {
    running = true;

    connectionCleaner = new std::thread([this](){
            while (running) {
                std::this_thread::sleep_for(std::chrono::seconds(5));
                vector<int> toRemove;
                for (int i = 0; i < connections.size(); i++)
                    if (this->connections[i]->isRunning() == false)
                        toRemove.push_back(i);
                for (int i = 0; i < toRemove.size(); i++)
                    this->connections.erase(
                            this->connections.begin() + toRemove[i]);
            }
        });
    int returnValue = -1;
    struct addrinfo *serverInfo, *p;
    int yes=1;

    if ((returnValue = getaddrinfo(NULL, port.c_str(), &hints, &serverInfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(returnValue));
        return;
    }


    int new_fd;
    for(p = serverInfo; p != NULL; p = p->ai_next) {
        // Create socket
        if ((socketFd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            continue;
        }

        if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            freeaddrinfo(serverInfo);
            return;
        }

        if (bind(socketFd, p->ai_addr, p->ai_addrlen) == -1) {
            close(socketFd);
            continue;
        }

        break;
    }

    freeaddrinfo(serverInfo); // all done with this structure

    if (p == NULL)  {
        cout << "server: failed to bind\n";
        return;
    }

    if (listen(socketFd, BACKLOG) == -1) {
        perror("listen");
        return;
    }

    printf("server: waiting for connections...\n");

    servingThread = new std::thread([this](){
            char s[INET6_ADDRSTRLEN];
            while(this->running) {
                struct sockaddr_storage their_addr;
                socklen_t sin_size = sizeof their_addr;

                int new_fd = accept(this->socketFd,
                    (struct sockaddr *)&their_addr, &sin_size);
                if (new_fd == -1) {
                    continue;
                }

                inet_ntop(their_addr.ss_family,
                    get_in_addr((struct sockaddr *)&their_addr),
                    s, sizeof s);
                printf("server: got connection from %s\n", s);

                shared_ptr<TCPConnection> connection = 
                    std::make_shared<TCPConnection>(new_fd);
                this->connections.push_back(connection);
                connection->setEventHandler(TCPConnection::Event::ON_MESSAGE,
                        [s](TCPConnection::EventArgument message) {
                            cout << "Received message from [" << s << "]: "
                                 << message << endl;
                        });
                connection->start();
            }
        });
}

void TCPServer::stop() {
    running = false;
    int returnValue = close(socketFd);
    for (int i = 0; i < connections.size(); ++i) {
        connections[i]->stop();
    }
    servingThread->join();
}

}
