#ifndef LOCKER_H
#define LOCKER_H

#include <mutex>
#include <condition_variable>

class Locker
{
private:
    bool done_;
    std::mutex mutex_;
    std::condition_variable condition_;

public:
    Locker(bool done):done_(done){}

    void wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock,[=](){return done_;});
        done_ = false;
    }

    void signal() {
        std::lock_guard<std::mutex> lock(mutex_);
        condition_.notify_one();
        done_ = true;
    }
};

#endif // LOCKER_H

