#ifndef __HLT_CONTAINERS_THREADSAFEQUEUE_H__
#define __HLT_CONTAINERS_THREADSAFEQUEUE_H__

#include <queue>

namespace hlt {

template<typename T>
class ThreadSafeQueue : public std::queue<T> {

};

}

#endif // __HLT_CONTAINERS_THREADSAFEQUEUE_H__
