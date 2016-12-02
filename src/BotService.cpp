#include "BotService.h"

namespace hlt {

std::map<std::string, BotHandler::Ptr> BotService::bots;
std::map<ClientHandler::Ptr, BotHandler::Ptr> BotService::bridges;


int BotService::RegisterBot(std::string name, BotHandler::Ptr bot) {
    if (bots.find(name) == bots.end()) {
        bots[name] = bot;
        return 0;
    } else {
        return -1;
    }
}

void BotService::UnregisterBot(std::string name) {
    auto iter = bots.find(name);
    if (iter != bots.end()) {
        bots.erase(iter);
    }
}

}
