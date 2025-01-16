#include <iostream>

template <typename T>
class SharedPtr {
private:
    T* ptr;               // Raw pointer to the managed object
    unsigned* refCount;   // Pointer to the reference count

public:
    // Constructor: Initializes with a raw pointer
    explicit SharedPtr(T* rawPtr = nullptr)
        : ptr(rawPtr)
        , refCount(rawPtr ? new unsigned(1) : nullptr)
    {}

    // Copy constructor: Increments the reference count
    SharedPtr(const SharedPtr& other)
        : ptr(other.ptr)
        , refCount(other.refCount)
    {
        if (refCount)
            ++(*refCount); // Increment reference count
    }

    // Destructor: Decrements the reference count and deletes resources if needed
    ~SharedPtr() {
        release();
    }

    // Overloaded assignment operator
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) { // Avoid self-assignment
            release();        // Release the current resource
            ptr = other.ptr;
            refCount = other.refCount;
            if (refCount)
                ++(*refCount); // Increment reference count
        }
        return *this;
    }

    // Dereference operator: Returns a reference to the managed object
    T& operator*() const {
        return *ptr;
    }

    // Member access operator: Returns the raw pointer
    T* operator->() const {
        return ptr;
    }

    // Check if the pointer is valid
    bool isValid() const {
        return ptr != nullptr;
    }

    // Get the current reference count
    unsigned use_count() const {
        return refCount ? *refCount : 0;
    }

private:
    // Releases the ownership of the current resource
    void release() {
        if (refCount) {
            --(*refCount);    // Decrement the reference count
            if (*refCount == 0) {
                delete ptr;    // Delete the managed object
                delete refCount; // Delete the reference count
            }
        }
        ptr = nullptr;
        refCount = nullptr;
    }
};

// Example usage
int main() {
    SharedPtr<int> sp1(new int(42));
    std::cout << "Value: " << *sp1 << ", Use count: " << sp1.use_count() << std::endl;

    SharedPtr<int> sp2 = sp1; // Copy constructor
    std::cout << "Value: " << *sp2 << ", Use count: " << sp2.use_count() << std::endl;

    SharedPtr<int> sp3;
    sp3 = sp1; // Assignment operator
    std::cout << "Value: " << *sp3 << ", Use count: " << sp3.use_count() << std::endl;

    return 0;
}
