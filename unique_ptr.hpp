#ifndef UNIQUE_PTR_HPP
#define UNIQUE_PTR_HPP

#include <utility>
template <typename T>
class UniquePtr {
private:
    T* _p;
public:
    //construct and deconstruct
    UniquePtr(T* ptr):_p(ptr) {}
    UniquePtr():_p(nullptr) {}
    ~UniquePtr() {
        reset();
    }

    //delete copy constructor and copy operator
    UniquePtr(UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    //move constructor
    UniquePtr(UniquePtr&& other):_p(other._p) {
        other._p = nullptr;
    }
    //move assignment operator
    UniquePtr& operator=(UniquePtr&& other) {
        if (this != &other) {
            delete _p;
            _p = other._p;
            other._p = nullptr;
        }
        return *this;
    }

    //get()
    T* get() const {
        return _p;
    }

    //dereference operator
    T& operator*() const {
        return *_p;
    }

    //arrow operator
    T* operator->() const {
        return _p;
    }

    //to bool
    operator bool() const {
        return _p != nullptr;
    }

    //length
    std::size_t length() const {
        return _p->length();
    }
    
    //reset
    void reset() {
        delete _p;
        _p = nullptr;
    }

    //reset to a new pointer
    void reset(T* ptr) {
        delete _p;
        _p = ptr;
    }

    //release
    T* release() {
        T* tmp = _p;
        _p = nullptr;
        return tmp;
    }
};


template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}
#endif