#include <arpa/inet.h>
#include <cstring>
#include <exception>
#include <unistd.h>

#include "TCPServer.h"

namespace hlt {

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

TCPServer::TCPServer(string port) : port(port)
{
    std::memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
}

TCPServer::~TCPServer()
{
    // Intentionally empty
}

void TCPServer::start()
{
    running = true;

    connectionCleaner = new std::thread(
        [this]()
        {
            while (running)
            {
                std::this_thread::sleep_for(std::chrono::seconds(5));
                for (int i = connections.size()-1; i >= 0 ; --i)
                    if (this->connections[i]->isRunning() == false)
                    {
                        std::cout << "Removing connection" << std::endl;
                        this->connections.erase(this->connections.begin() + i);           
                    }         
            }
        }
    );

    struct addrinfo* serverInfo;
    if (getaddrinfo(NULL, port.c_str(), &hints, &serverInfo) != 0)
    {
        throw TCPServerException("Failed to get address info");
    }

    int new_fd;
    struct addrinfo *p = createSocket(serverInfo);
    freeaddrinfo(serverInfo);
    if (p == NULL)
    {
        throw TCPServerException("Couldn't create socket");
    }

    if (listen(socketFd, BACKLOG) == -1)
    {
        throw TCPServerException("Failed to listen socket");
    }

    string eventMessage = "Server is up on port " + port;
    eventEmitter.emit("open", {NULL, eventMessage});

    servingThread = new std::thread(
        [this]()
        {
            char s[INET6_ADDRSTRLEN];
            while(this->running)
            {
                struct sockaddr_storage their_addr;
                socklen_t sin_size = sizeof their_addr;

                int new_fd = accept(this->socketFd,
                    (struct sockaddr *)&their_addr, &sin_size);
                if (new_fd == -1)
                {
                    continue;
                }

                inet_ntop(their_addr.ss_family,
                    get_in_addr((struct sockaddr *)&their_addr),
                    s, sizeof s);

                TCPConnection::Ptr connection(new TCPConnection(new_fd));
                this->connections.push_back(connection);
                connection->start();

                EventArgument arg;
                arg.connection = connection;
                eventEmitter.emit("connection", arg);
            }
        }
    );
}

void TCPServer::stop()
{
    running = false;
    int returnValue = close(socketFd);
    for (int i = 0; i < connections.size(); ++i)
    {
        connections[i]->stop();
    }
    connections.clear();
    servingThread->join();
    connectionCleaner->join();
}

EventEmitter<TCPServer::EventArgument>& TCPServer::getEventEmitter()
{
    return this->eventEmitter;
}

struct addrinfo* TCPServer::createSocket(struct addrinfo* serverInfo)
{
    struct addrinfo *p;
    for(p = serverInfo; p != NULL; p = p->ai_next)
    {
        // Create socket
        if ((socketFd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1)
        {
            continue;
        }

        int yes = 1;
        if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1)
        {
            freeaddrinfo(serverInfo);
            close(socketFd);
            throw TCPServerException("Failed to set socket option SO_REUSEADDR");
        }

        if (bind(socketFd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(socketFd);
            continue;
        }

        break;
    }
    return p;
}

}
