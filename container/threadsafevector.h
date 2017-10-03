#ifndef THREADSAFECONTAINER_H
#define THREADSAFECONTAINER_H

#include <vector>
#include <mutex>
#include <algorithm>

struct Bubble {
    float x;    // координата по x
    float y;    // координата по y
    float vx;   // скорость вдоль оси x
    float vy;   // скорость вдоль оси y

    Bubble(float x, float y):x(x),y(y),vx(0),vy(0){}
    bool operator==(const std::pair<float,float>& point){return point.first == x && point.second == y;}
};

template <class T> class ThreadSafeVector
{
private:
    std::vector<T> vector_;        // вектор элементов
    mutable std::mutex mutex_;     // мьютекс для синхронизации доступа

public:
    ThreadSafeVector();
    ~ThreadSafeVector();
    void addElement(const T& obj);
    void removeElement(const T& obj);
    void setElement(const size_t index, const T& newValue);
    std::vector<T> getElements() const;
    size_t getElementsCount() const;
};

template <class T> ThreadSafeVector<T>::ThreadSafeVector()
{

}

template <class T> ThreadSafeVector<T>::~ThreadSafeVector()
{

}

// добавить элемент в контейнер
template <class T> void ThreadSafeVector<T>::addElement(const T& obj)
{
    std::lock_guard<std::mutex> lock(mutex_);
    vector_.push_back(obj);
}

// удалить элемент из контейнера
template <typename T> void ThreadSafeVector<T>::removeElement(const T& obj)
{
    std::lock_guard<std::mutex> lock(mutex_);

    auto element = std::find(vector_.begin(),vector_.end(),obj);
    vector_.erase(element);
}

// изменить элемент
template <class T> void ThreadSafeVector<T>::setElement(const size_t index, const T& newValue)
{
    std::lock_guard<std::mutex> lock(mutex_);

    vector_[index] = newValue;
}

// получить все элементы контейнера
template <class T> std::vector<T> ThreadSafeVector<T>::getElements() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return vector_;
}

template <class T> size_t ThreadSafeVector<T>::getElementsCount() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return vector_.size();
}

#endif // THREADSAFECONTAINER_H
