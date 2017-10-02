#ifndef THREADSAFECONTAINER_H
#define THREADSAFECONTAINER_H

#include <vector>
#include <mutex>
#include <algorithm>

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
    void updateElement(const T& oldValue, const T& newValue);
    std::vector<T> getElements() const;
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
template <class T> void ThreadSafeVector<T>::updateElement(const T& oldValue, const T& newValue)
{
    std::lock_guard<std::mutex> lock(mutex_);

    auto element = std::find(vector_.begin(),vector_.end(),oldValue);
    *element = newValue;
}

// получить все элементы контейнера
template <class T> std::vector<T> ThreadSafeVector<T>::getElements() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return vector_;
}

#endif // THREADSAFECONTAINER_H
