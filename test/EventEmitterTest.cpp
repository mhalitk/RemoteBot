#include "gtest/gtest.h"

#include "EventEmitter.hpp"

typedef std::string EventArgument;

TEST(EventEmitterTest, CreateTest) {
    hlt::EventEmitter<EventArgument> eventEmitter;
    SUCCEED();
}