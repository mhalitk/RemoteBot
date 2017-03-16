#include <iostream>

#include "ClientHandler.h"

namespace hlt {

ClientHandler::ClientHandler(TCPConnection::Ptr connection) :
        connection(connection)
{
    connection->getEventEmitter().on("message", std::bind(
                &ClientHandler::onMessage, this, std::placeholders::_1));
}

ClientHandler::~ClientHandler() {}

void ClientHandler::onMessage(const TCPConnection::EventArgument& arg)
{
    std::cout << "Message received: " << arg << std::endl;
}

}
