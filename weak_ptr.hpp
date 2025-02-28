#ifndef WEAK_PTR_HPP
#define WEAK_PTR_HPP
#include <cstddef>
#include <utility>
#include <unordered_set>
#include <iostream>

template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr;

class Ctrl {
    template <typename T>
    friend class SharedPtr;
    template <typename T>
    friend class WeakPtr;
private:
    int countsp = 0;
    int countwp = 0;
public:
    Ctrl(const Ctrl& other):countwp(other.countwp), countsp(other.countsp) {}
    Ctrl &operator=(const Ctrl& other) {
            countsp = other.countsp;
            countwp = other.countwp;
        return *this;
    }
    Ctrl():countsp(0), countwp(0) {}
    Ctrl(Ctrl&& other) {
        countsp = other.countsp;
        countwp = other.countwp;
        other.countsp = 0;
        other.countwp = 0;
    }
    Ctrl &operator=(Ctrl&& other) {
        
            countsp = other.countsp;
            countwp = other.countwp;
            other.countsp =0;
            other.countwp = 0;
        
        return *this;
    }
    ~Ctrl() {
        countsp = 0;
        countwp = 0;
    }
};

class CtrlManage {
private:
    std::unordered_set<Ctrl*> all;
public:
    void add(Ctrl* ctrl) {
        all.insert(ctrl);
    }
    void erase(Ctrl* ctrl) {
        all.erase(ctrl);
    }
    ~CtrlManage() {
        for (auto i: all) {
            delete i;
            i = nullptr;
        }
        std::unordered_set<Ctrl*>().swap(all);
    }
};

CtrlManage all;

template <typename T>
class SharedPtr {
    friend class WeakPtr<T>;
private:
    Ctrl* ctrl = nullptr;
    T*_p;
public:
    //construct and deconstruct
    SharedPtr(T* ptr):_p(ptr) {
        if (ptr != nullptr) {
            ctrl = new Ctrl();
            all.add(ctrl);
            ctrl->countsp = 1;
            ctrl->countwp = 0;
        }
    }
    SharedPtr():_p(nullptr) {}
    ~SharedPtr() { reset(); }

    //copy constructor
    SharedPtr(const SharedPtr& other):_p(other._p), ctrl(other.ctrl) {
        if (ctrl != nullptr)  ++(ctrl->countsp);
    }
    //copy assignment operator
    SharedPtr &operator=(SharedPtr& other){
        if (this != &other) {
            if (ctrl != nullptr && --(ctrl->countsp) <= 0) {
                delete _p;
            }
            _p = other._p;
            ctrl = other.ctrl;
            if (ctrl != nullptr)  ++(ctrl->countsp);
        }
        return *this;
    }

    //move constructor
    SharedPtr(SharedPtr&& other) noexcept :_p(other._p), ctrl(other.ctrl) {
        other._p = nullptr;
        other.ctrl = nullptr;
    }
    //move assignment operator
    SharedPtr &operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            if (ctrl != nullptr  && --(ctrl->countsp) <= 0) {
                delete _p;
            }
            _p = other._p;
            ctrl = other.ctrl;
            other._p = nullptr;
            other.ctrl = nullptr;
        }
        return *this;
    }

    //count pointers pointing to the same memory
    int use_count() const {
        if (ctrl != nullptr)  return ctrl->countsp;
        return 0;
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
        if (ctrl != nullptr && --ctrl->countsp <= 0) {
            delete _p;
            if (ctrl->countwp <= 0)  {delete ctrl;  all.erase(ctrl);}
        }
        _p = nullptr;
        ctrl = nullptr;
    }

    //reset to a new pointer
    void reset(T* ptr) {
        if (ctrl != nullptr && --(ctrl->countsp) <= 0) {
            delete _p;
            ctrl = nullptr;
        }
        _p = ptr;
        if (ptr != nullptr) {
            ctrl = new Ctrl();
            all.add(ctrl);
            ctrl->countsp = 1;
        } else {
            ctrl = nullptr;
        }
    }
};

template <typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
class WeakPtr {
private:
    Ctrl* ctrl = nullptr;
    T* _wp = nullptr;
public:
    //construct and deconstruct
    WeakPtr(const SharedPtr<T>& sp) noexcept :_wp(sp._p) , ctrl(sp.ctrl) {
        if (ctrl != nullptr) {
            ++ctrl->countwp;
        }
    }
    WeakPtr() noexcept :_wp(nullptr), ctrl(nullptr) {}
    ~WeakPtr() { reset(); }

    //copy constructor
    WeakPtr(WeakPtr& other) noexcept :_wp(other._wp), ctrl(other.ctrl) {
        _wp = other._wp;
        ctrl = other.ctrl;
        if (ctrl != nullptr)  ++ctrl->countwp;
    }
    //copy assignment operator
    WeakPtr &operator=(WeakPtr& other) noexcept {
        if (this != &other) {
            _wp = other._wp;
            ctrl = other.ctrl;
            if (ctrl != nullptr)  ++ctrl->countwp;
        }
        return *this;
    }
    WeakPtr &operator=(const SharedPtr<T>& sp) noexcept {
        _wp = sp._p;
        ctrl = sp.ctrl;
        if (ctrl != nullptr) {
            ++ctrl->countwp;
        }
        return *this;
    }

    //move constructor
    WeakPtr(WeakPtr&& other) noexcept :_wp(other._wp), ctrl(other.ctrl) {
        other._wp = nullptr;
        other.ctrl = nullptr;
    }
    //move assignment operator
    WeakPtr &operator=(WeakPtr&& other) noexcept {
        if (this != &other) {
            _wp = other._wp;
            ctrl = other.ctrl;
            other._wp = nullptr;
            other.ctrl = nullptr;
        }
        return *this;
    }

    //expire
    bool expired() {
        if (ctrl == nullptr || (ctrl != nullptr && ctrl->countsp <= 0)) {
            return true;
        }
        return false;
    }

    //lock
    SharedPtr<T> lock() {
        if (!expired()) {
            if (ctrl != nullptr) {
                ++ctrl->countsp;
            }
            SharedPtr<T> tmp;
            tmp._p = _wp;
            tmp.ctrl = ctrl;
            return tmp;
        } else {
            return SharedPtr<T>();
        }
    }

    //use_count
    int use_count() {
        if (!expired() && ctrl != nullptr)  return ctrl->countsp;
        return 0;
    }

    //reset
    void reset() {
        if (ctrl != nullptr && ctrl->countsp <= 0) {
            if (--ctrl->countwp <= 0)  {delete ctrl;  all.erase(ctrl);}
        }
        _wp = nullptr;
        ctrl = nullptr;
    }

    //swapa
    void swap(WeakPtr& other) {
        std::swap(_wp, other._wp);
        std::swap(ctrl, other.ctrl);
    }
};

#endif