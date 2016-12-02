#ifndef __HLT_EVENTEMITTER_HPP__
#define __HLT_EVENTEMITTER_HPP__

#include <iostream>
#include <future>
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
            eventMap[eventName] = std::make_shared<std::vector<EventCallback>>();
        eventMap[eventName]->push_back(callback); 
    }

    void emit(std::string eventName, const T& eventArgument) {
        try {
            if (eventMap.find(eventName) != eventMap.end()) {
                std::shared_ptr<std::vector<EventCallback>> callbacks = eventMap[eventName];
                for (auto callback = callbacks->begin();
                        callback != callbacks->end();
                        callback++) {
                    (*callback)(eventArgument);
                }
            }
        } catch (...) {
            // Do nothing
        }
    }

    void clearAll() {
        eventMap.clear();
    }

private:
    std::map<std::string, std::shared_ptr<std::vector<EventCallback>>> eventMap;
};

}

#endif
