#ifndef MY_UNIQUE_PTR_H
#define MY_UNIQUE_PTR_H

#include <utility>

template <typename T>  /* single object ownership */
class MyUniquePtr {
private:
    T* _ptr;  // pointer to the managed resource

public:
    MyUniquePtr() : _ptr(nullptr) {}

    // constructor accepting a raw pointer
    explicit MyUniquePtr(T* ptr) : _ptr(ptr) {}

    // delete copy constructor to prevent copying
    MyUniquePtr(const MyUniquePtr&) = delete;

    // move constructor: transfers ownership
    MyUniquePtr(MyUniquePtr&& other) noexcept : _ptr(other._ptr) {
        other._ptr = nullptr;  // nullify the source pointer
    }

    // move assignment operator: transfers ownership
    MyUniquePtr& operator=(MyUniquePtr&& other) noexcept {
        if (this != &other) {     // prevent self-assignment
            delete _ptr;          // release existing resource
            _ptr = other._ptr;    // take ownership
            other._ptr = nullptr; // nullify the source pointer
        }
        return *this;
    }

    // Destructor: releases the managed resource
    ~MyUniquePtr() {
        delete _ptr;
    }

    // dereference operator to access the managed object
    T& operator*() const {
        return *_ptr;
    }

    // member access operator to access the managed object's members
    T* operator->() const {
        return _ptr;
    }

    // returns the raw pointer (without transferring ownership)
    T* Get() const {
        return _ptr;
    }

    // transfers ownership and nullifies the internal pointer
    T* Release() {
        T* temp = _ptr;
        _ptr = nullptr;
        return temp;
    }

    // delete copy assignment operator to prevent copying
    MyUniquePtr& operator=(const MyUniquePtr&) = delete;
};


template <typename T>  /* specialization for arrays */
class MyUniquePtr<T[]> {
private:
    T* _ptr;  // pointer to the managed array

public:
    MyUniquePtr() : _ptr(nullptr) {}

    // constructor accepting a raw array pointer
    explicit MyUniquePtr(T* ptr) : _ptr(ptr) {}

    // delete copy constructor to prevent copying
    MyUniquePtr(const MyUniquePtr&) = delete;

    // move constructor
    MyUniquePtr(MyUniquePtr&& other) noexcept : _ptr(other._ptr) {
        other._ptr = nullptr; // nullify the source pointer
    }

    // move assignment operator
    MyUniquePtr& operator=(MyUniquePtr&& other) noexcept {
        if (this != &other) {
            delete[] _ptr;         // release existing array resource
            _ptr = other._ptr;     // take ownership
            other._ptr = nullptr;  // nullify the source pointer
        }
        return *this;
    }

    ~MyUniquePtr() {
        delete[] _ptr;
    }

    // subscript operator to access array elements
    T& operator[](std::size_t index) const {
        return _ptr[index];
    }

    T* Get() const {
        return _ptr;
    }

    T* Release() {
        T* temp = _ptr;
        _ptr = nullptr;
        return temp;
    }

    // delete copy assignment operator
    MyUniquePtr& operator=(const MyUniquePtr&) = delete;
};

#endif // MY_UNIQUE_PTR_H
