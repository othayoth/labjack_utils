#ifndef UTILITY
#define UTILITY
// C++ implementation of the above approach
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>

struct ThreadMessage {
    int action;
    int obj_idx;
};

// Thread-safe queue
template <typename T>
class TSQueue {
private:
    // Underlying queue
    std::vector<T> m_queue;
  
    // mutex for thread synchronization
    std::mutex m_mutex;
  
    // Condition variable for signaling
    std::condition_variable m_cond;
  
public:
    // Pushes an element to the queue
    void push(T item)
    {
  
        // Acquire lock
        std::unique_lock<std::mutex> lock(m_mutex);
  
        // Add item
        m_queue.push_back(item);
  
        // Notify one thread that
        // is waiting
        m_cond.notify_one();
    }
  
    // Pops an element off the queue
    T pop()
    {
  
        // acquire lock
        std::unique_lock<std::mutex> lock(m_mutex);
  
        // wait until queue is not empty
        m_cond.wait(lock,
                    [this]() { return !m_queue.empty(); });
  
        // retrieve item
        T item = m_queue.front();
        m_queue.erase(m_queue.begin());
        // return item
        return item;
    }
    T size()
    {
        return m_queue.size();
    }
    bool empty()
    {
        return m_queue.empty();
    }
    T front()
    {
        return m_queue.front();
    }
    T at(int idx) 
    {
        return m_queue[idx];
    }
};

inline float tick()
{
    struct timespec ts;
    uint32_t res = clock_gettime(CLOCK_MONOTONIC, &ts);
    if (res == -1)
    {
        return 0;
    }
    return ((float)((ts.tv_sec * 1e9) + ts.tv_nsec)) / (float)1.0e9;
}

#endif
