#ifndef __HLT_SERVICEPROVIDERHANDLER_H__
#define __HLT_SERVICEPROVIDERHANDLER_H__

#include <memory>

#include "Net/TCPConnection.h"

namespace hlt {

class ServiceProviderHandler {
public:
    typedef std::shared_ptr<ServiceProviderHandler> Ptr;

    ServiceProviderHandler(TCPConnection::Ptr connection);
    ~ServiceProviderHandler();

    void onMessage(const TCPConnection::EventArgument& arg);

private:
    TCPConnection::Ptr connection;
};

}

#endif
