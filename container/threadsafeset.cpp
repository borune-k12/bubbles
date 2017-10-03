#include "threadsafeset.h"

ThreadSafeSet::ThreadSafeSet()
{

}

ThreadSafeSet::~ThreadSafeSet()
{

}

// добавить элемент в контейнер
void ThreadSafeSet::addElement(const Point &obj)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    set_.insert(obj);
}

// удалить элемент из контейнера
bool ThreadSafeSet::removeElement(const Point &obj)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    return set_.erase(obj);
}

// изменить элемент
void ThreadSafeSet::updateElement(const Point& oldValue, const Point& newValue)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    if(removeElement(oldValue))
        addElement(newValue);
}

// получить все элементы контейнера
std::unordered_set<Point,pair_hash> ThreadSafeSet::getElements() const
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return set_;
}

std::unordered_set<Point,pair_hash>::size_type ThreadSafeSet::getElementsCount() const
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return set_.size();
}

std::vector<Point> ThreadSafeSet::getNeighbours(const Point &point) const
{
    std::vector<Point> result;
    for(auto it=set_.begin(); it != set_.end(); ++it)
        if(*it != point)
            result.push_back(*it);

    return result;
}


