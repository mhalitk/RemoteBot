#include <iostream>
#include <future>
#include <signal.h>
#include <string.h>
#include <vector>

#include "BotHandler.h"
#include "BotService.h"
#include "config.h"
#include "ClientHandler.h"
#include "Net/TCPConnection.h"
#include "Net/TCPServer.h"

using namespace std;
using namespace hlt;

vector<ClientHandler::Ptr> clientHandlers;
vector<BotHandler::Ptr> botHandlers;

void removeBot(BotHandler::Ptr handler)
{
    BotService::UnregisterBot(handler->getName());
    auto iter = std::find(botHandlers.begin(), botHandlers.end(), handler);
    if (iter != botHandlers.end())
        botHandlers.erase(iter);
}

void registerBot(const BotHandler::EventArgument& arg, BotHandler::Ptr botHandler)
{
    cout << "Trying to register new bot: " << arg["name"] << endl;
    int result = BotService::RegisterBot(arg["name"], botHandler);
    if (result == 0)
    {
        cout << "Registered new bot: " << arg["name"] << endl;
    }
    else
    {
        cout << "Couldn't register bot closing..." << endl;
        botHandler->close();
        removeBot(botHandler);
    }
}

static void signalHandler(int signum)
{
    // TODO Handle signals
}

int main(int argc, char** argv)
{
    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGPIPE, &sa, NULL) == -1)
    {
        cout << "Couldn't set signal handler. Exiting..." << endl;
        return 1;
    }

    TCPServer clientServer(string(CLIENT_PORT));
    clientServer.getEventEmitter().on("connection",
        [](const TCPServer::EventArgument& arg) {
            cout << "New connection to client server!" << endl;
            clientHandlers.push_back(ClientHandler::Ptr(
                        new ClientHandler(arg.connection))); 
        }
    );
    clientServer.start();

    TCPServer spServer(string(SP_PORT));
    spServer.getEventEmitter().on("connection", 
        [](const TCPServer::EventArgument& arg) {
            cout << "New connection to bot server!" << endl;
            BotHandler::Ptr botHandler(new BotHandler(arg.connection));
            botHandlers.push_back(botHandler);
            botHandler->getEventEmitter().on("close", std::bind(removeBot, botHandler));
            botHandler->getEventEmitter().on("info", std::bind(registerBot, placeholders::_1, botHandler));
        }
    );
    spServer.start();

    while (1)
    {
        string in;
        cin >> in;
        // Possible UI here
    }

    return 0;
}
