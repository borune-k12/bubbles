#ifndef LOCKER_H
#define LOCKER_H

#include <mutex>
#include <condition_variable>

/** класс одноразрядного семафора */
class Locker
{
private:
    bool occupied_;
    std::mutex mutex_;
    std::condition_variable condition_;

public:
    explicit Locker(bool done):occupied_(done){}

    void wait() {
        std::unique_lock<std::mutex> lock(mutex_);

        // защита от случайного пробуждения
        condition_.wait(lock,[this](){return occupied_;});
        occupied_ = false;
    }

    void signal() {
        std::lock_guard<std::mutex> lock(mutex_);
        condition_.notify_one();
        occupied_ = true;
    }
};

#endif // LOCKER_H
