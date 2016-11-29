#ifndef __HLT_CLIENTHANDLER_H__
#define __HLT_CLIENTHANDLER_H__

#include "Net/TCPConnection.h"

namespace hlt {

class ClientHandler {
public:
    typedef std::shared_ptr<ClientHandler> Ptr;

    ClientHandler(TCPConnection::Ptr connection);
    ~ClientHandler();

    void onMessage(const TCPConnection::EventArgument& arg);
private:
    TCPConnection::Ptr connection;
};

}

#endif
