#ifndef __HLT_NET_TCPCLIENT_H__
#define __HLT_NET_TCPCLIENT_H__

#include <functional>
#include <map>
#include <string>
#include <thread>
#include <vector>

#include "Containers/ThreadSafeQueue.h"

namespace hlt {

/**
 * class TCPConnection
 * -----
 *  This class handles a tcp connection sending and receiving tasks. It creates
 *  two threads for handling send/receive. Needs file descriptor of TCP socket.
 *  If socket is closed, closes threads and set running flag to false. It should
 *  be stopped after its running state changed to false.
 */
class TCPConnection {
public:
    typedef std::string EventArgument;
    typedef std::function<void(EventArgument)> EventHandler;
    typedef std::shared_ptr<TCPConnection> Ptr;

    enum Event {
        ON_MESSAGE,
        ON_CLOSE
    };
    
    /**
     * Constructor, sets socket descriptor
     */
    TCPConnection(int connectionDescriptor);
    ~TCPConnection();

    /**
     * Starts send/receive threads, sets running state to true.
     */
    void start();
    /**
     * Sets running state to false and waits send/receive threads to join
     */
    void stop();

    /**
     * Adds given message to sending queue
     *
     * @param message Message to send
     */
    void sendMessage(const std::string& message);
    /**
     * Sets event handler for specific event. Events are defined in TCPConnection::Event
     * enum.
     *
     * @param event Target event of handler
     * @param handler Handler function for event
     */
    void setEventHandler(TCPConnection::Event event, EventHandler handler);

    /**
     * Returns running state of connection
     *
     * @return Running state
     */
    bool isRunning(); 
private:
    void startSending();
    void startReceiving();

    int connectionDescriptor = -1;
    bool running = false;

    std::thread* sendingThread;
    std::thread* receivingThread;

    std::map<Event, std::vector<EventHandler>> handlers;
    ThreadSafeQueue<std::string> sendingQueue;
};

}

#endif
