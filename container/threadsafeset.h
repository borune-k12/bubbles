#ifndef THREADSAFESET_H
#define THREADSAFESET_H

#include <unordered_set>
#include <mutex>

#define MAX_X 1920;

struct pair_hash {
    inline std::size_t operator()(const std::pair<float,float> & v) const {
        return v.first*MAX_X+v.second;
    }
};

typedef std::pair<float,float> Point;

class ThreadSafeSet
{
private:
    std::unordered_set<Point,pair_hash> set_;    // набор элементов
    mutable std::recursive_mutex mutex_;     // мьютекс для синхронизации доступа

public:
    ThreadSafeSet();
    ~ThreadSafeSet();
    void addElement(const Point& obj);
    bool removeElement(const Point& obj);
    void updateElement(const Point& oldValue, const Point& newValue);
    std::unordered_set<Point, pair_hash> getElements() const;
};



#endif // THREADSAFESET_H
