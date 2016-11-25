#ifndef __HLT_EVENTEMITTER_HPP__
#define __HLT_EVENTEMITTER_HPP__

#include <iostream>
#include <map>
#include <vector>

namespace hlt {

template<typename T>
class EventEmitter {
public:
    typedef std::function<void(const T&)> EventCallback;

    EventEmitter() {}
    
    void on(std::string eventName, EventCallback callback) {
        if (eventMap.find(eventName) == eventMap.end())
            eventMap[eventName] = new std::vector<EventCallback>;
        eventMap[eventName]->push_back(callback); 
    }

    void emit(std::string eventName, const T& eventArgument) {
        if (eventMap.find(eventName) != eventMap.end()) {
            for (auto callback = eventMap[eventName]->begin();
                    callback != eventMap[eventName]->end();
                    callback++) {
                (*callback)(eventArgument);
            }
        }
    }

private:
    std::map<std::string, std::vector<EventCallback>*> eventMap;
};

}

#endif
