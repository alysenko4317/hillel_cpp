#include <iostream>
#include <memory>
#include <mutex>

class Singleton {
public:
    static Singleton& getInstance() {
        std::call_once(initFlag, []() {
            instance.reset(new Singleton());
        });
        return *instance;
    }

    void doSomething() {
        std::cout << "Singleton is working!" << std::endl;
    }

    ~Singleton() = default;

private:
    Singleton() {
        std::cout << "Singleton instance created." << std::endl;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static std::unique_ptr<Singleton> instance;
    static std::once_flag initFlag;
};

std::unique_ptr<Singleton> Singleton::instance;
std::once_flag Singleton::initFlag;

int main() {
    Singleton& s1 = Singleton::getInstance();
    s1.doSomething();

    Singleton& s2 = Singleton::getInstance();
    s2.doSomething();

    return 0;
}
