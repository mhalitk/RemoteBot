#ifndef __HLT_SERVICEPROVIDERHANDLER_H__
#define __HLT_SERVICEPROVIDERHANDLER_H__

#include <map>
#include <memory>

#include "EventEmitter.hpp"
#include "json.hpp"
#include "Net/TCPConnection.h"

using nlohmann::json;

namespace hlt {

class BotHandler {
public:
    typedef std::shared_ptr<BotHandler> Ptr;
    typedef std::function<void(json)> Callback;
    typedef json EventArgument;

    BotHandler(TCPConnection::Ptr connection);
    ~BotHandler();

    void onMessage(const TCPConnection::EventArgument& arg);
    EventEmitter<EventArgument>& getEventEmitter();
    std::string getName();
    
    void close();
private:
    TCPConnection::Ptr connection;
    std::map<int, Callback> callbacks;
    EventEmitter<EventArgument> eventEmitter;

    int lastMessageId = 1;
    json createMessage(const std::string& type, const std::string& command);
    std::string name;
};

}

#endif
