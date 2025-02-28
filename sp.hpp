#ifndef SHARED_PTR
#define SHARED_PTR

#include <utility>

template <typename T>
class SharedPtr
{
public:
    SharedPtr() {
        _p = nullptr;
        count = nullptr;
    };
    SharedPtr(T* p) {
        _p = p;
        if(_p != nullptr)
            count = new int(1);
        else
            count = nullptr;
    };
    ~SharedPtr() {
        release();
    };
    SharedPtr(const SharedPtr & other) {
        release();
        _p = other._p;
        if(_p) {
            count = other.count;
            if(count)
                (*count)++;
        }else
            count = nullptr;
    };
    SharedPtr(SharedPtr && other) {
        release();
        _p = other._p;
        if(_p)
            count = other.count;
        else
            count = nullptr;
        other._p = nullptr;
        other.count = nullptr;
    };
    SharedPtr& operator= (const SharedPtr & other) {
        if(this == &other)
            return *this;
        release();
        _p = other._p;
        if(_p){
            count = other.count;
            if (count != nullptr)  (*count)++;
        }else {
            count = nullptr;
        }
        return *this;
    };
    SharedPtr& operator= (SharedPtr && other) {
        if (this == &other) {
            return *this;
        }
        release();
        _p = other._p;
        if(_p)
            count = other.count;
        else
            count = nullptr;
        other._p = nullptr;
        other.count = nullptr;
        return *this;
    }
    int length() {
        if(_p)
            return _p->length();
        return 0;
    }
    operator bool() const{
        if(_p != nullptr && count != nullptr && *count != 0)
            return true;
        return false;
    };
    std::size_t use_count() const {
        if(count && _p)
            return *count;
        return 0;
    };
    T* get() const {
        return _p;
    };
    T& operator* () const {
        return *_p;
    };
    T* operator->() const {
        return _p;
    };
    void reset() {
        release();
        _p = nullptr;
        count = nullptr;
    };
    void reset(T* p) {
        if(_p == p)
            return;
        release();
        _p = p;
        if(p != nullptr)
            count = new int (1);
        else
            count = nullptr;
        //delete p;
    };
private:
   //add whatever u want bro
    T *_p = nullptr;
    int* count = nullptr;
    void release() {
        if(count && _p) {
            (*count)--;
            if(!(*count)) {
                delete _p;
                delete count;
            }
        }
        _p = nullptr;
        count = nullptr;
    }
};

//Then implement the make_shared function
//I think I'd better leave you to design the prototype :)

//template <?>
//? make_shared(?);
template <typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}
#endif //SHARED_PTR