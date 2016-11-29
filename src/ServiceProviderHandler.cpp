#include "ServiceProviderHandler.h"

namespace hlt {

ServiceProviderHandler::ServiceProviderHandler(TCPConnection::Ptr connection) :
        connection(connection) {
    connection->getEventEmitter().on("message", std::bind(
                &ServiceProviderHandler::onMessage, this, std::placeholders::_1));
    connection->sendMessage("{\"id\": \"1\", \"type\":\"request\", \"command\":\"getName\"}");
}

ServiceProviderHandler::~ServiceProviderHandler() {}

void ServiceProviderHandler::onMessage(const TCPConnection::EventArgument& arg) {

}

}
