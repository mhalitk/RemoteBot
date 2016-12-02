#ifndef __HLT_BOTSERVICE_H__
#define __HLT_BOTSERVICE_H__

#include <map>
#include <vector>

#include "BotHandler.h"
#include "ClientHandler.h"

namespace hlt {

class BotService {
public:
    static int RegisterBot(std::string name, BotHandler::Ptr);
    static void UnregisterBot(std::string name);
    static std::vector<std::string> GetBotNames();
private:
    BotService();

    static std::map<std::string, BotHandler::Ptr> bots;
    static std::map<ClientHandler::Ptr, BotHandler::Ptr> bridges;
};

}

#endif
