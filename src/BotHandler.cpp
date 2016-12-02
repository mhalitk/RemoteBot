#include "json.hpp"
#include "BotHandler.h"
#include "BotService.h"

using namespace std;

namespace hlt {

BotHandler::BotHandler(TCPConnection::Ptr connection) :
        connection(connection) {
    connection->getEventEmitter().on("message", std::bind(
                &BotHandler::onMessage, this, std::placeholders::_1));

    json getNameRequest = createMessage("request", "getName");
    callbacks[getNameRequest["id"]] = [this](json response){
            if (response["success"]) {
                this->name = response["name"];
                eventEmitter.emit("info", response);
            }
        };
    connection->sendMessage(getNameRequest.dump());
}

BotHandler::~BotHandler() {
    cout << "Destroying bot handler" << endl;
    cout << "TCPConnection use count: " << connection.use_count() << endl;
}

void BotHandler::onMessage(const TCPConnection::EventArgument& arg) {
    json message = json::parse(arg);
    if (message["type"] == "response" &&
            callbacks.find(message["request_id"]) != callbacks.end()) {
        callbacks[message["request_id"]](message);
    }
}

EventEmitter<BotHandler::EventArgument>& BotHandler::getEventEmitter() {
    return this->eventEmitter;
}

std::string BotHandler::getName() {
    return this->name;
}

void BotHandler::close() {
    eventEmitter.emit("close", "");
    eventEmitter.clearAll();
}

json BotHandler::createMessage(std::string type, std::string command) {
    json message;
    message["id"] = lastMessageId++;
    message["command"] = command;
    message["type"] = type;
    return message;
}

}
