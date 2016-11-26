#ifndef __HLT_NET_TCPCONNECTION_H__
#define __HLT_NET_TCPCONNECTION_H__

#include <functional>
#include <map>
#include <string>
#include <thread>

#include "Containers/ThreadSafeQueue.h"
#include "EventEmitter.hpp"

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
     * Returns event emitter object
     *
     * @return Event emitter
     */
    EventEmitter<EventArgument>& getEventEmitter();
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

    std::shared_ptr<std::thread> sendingThread;
    std::shared_ptr<std::thread> receivingThread;

    EventEmitter<EventArgument> eventEmitter;
    ThreadSafeQueue<std::string> sendingQueue;
};

}

#endif
