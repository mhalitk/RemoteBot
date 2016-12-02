#include <cerrno>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#include "TCPConnection.h"

using namespace std;

namespace hlt {

TCPConnection::TCPConnection(int connectionDescriptor):
        connectionDescriptor(connectionDescriptor) {
}

TCPConnection::~TCPConnection() {
    cout << "Destroying TCPConnection" << endl;
    if (isRunning()) {
        stop();
    }
}

void TCPConnection::start() {
    running = true;
    sendingThread = new std::thread([this](){ this->startSending(); });
    receivingThread = new std::thread([this](){ this->startReceiving(); });
}

void TCPConnection::stop() {
    running = false;

    if (sendingThread != nullptr)
        sendingThread->join();

    if (receivingThread != nullptr)
        receivingThread->join();

    close(connectionDescriptor);
    eventEmitter.clearAll();
}

void TCPConnection::sendMessage(const std::string& message) {
    sendingQueue.push(message);
}

EventEmitter<TCPConnection::EventArgument>& TCPConnection::getEventEmitter() {
    return this->eventEmitter;
}

bool TCPConnection::isRunning() {
    return this->running;
}

void TCPConnection::startSending() {
    int sendFlags = 0;

    while (running) {
        if (sendingQueue.size() == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        string message = sendingQueue.front();
        if (send(this->connectionDescriptor, message.c_str(), message.size(), sendFlags) == -1) {
            if (errno == EPIPE) {
                break;
            }
        } else {
            sendingQueue.pop();
        }
    }

    running = false;
}

void TCPConnection::startReceiving() {
    char buffer[1024];
    int recvFlags = 0;

    struct pollfd recvFd[1];
    recvFd[0].fd = this->connectionDescriptor;
    recvFd[0].events = POLLIN | POLLRDNORM | POLLRDBAND;

    while (running) {
        if (poll(recvFd, 1, 500) <= 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        int receivedLength = recv(this->connectionDescriptor, buffer, 1024, recvFlags);
        // If there is an error on socket close connection
        if (receivedLength == 0 || receivedLength == -1) {
            break;
        }
        buffer[receivedLength] = '\0';

        eventEmitter.emit("message", EventArgument(buffer));
    }

    running = false;
}

}
