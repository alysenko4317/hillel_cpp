
#include <set>
#include <vector>
#include <string>
#include <iostream>

class MyClass {
private:
    int _id;
    std::string _name;
    std::vector<int> _data;

public:

    MyClass()  /* default constructor */
        : _id(0)
        , _name("Default")
        , _data({})
    {
        std::cout << "Default constructor called\n";
    }

    MyClass(int id, std::string name, std::vector<int> data)
        : _id(id)
        , _name(std::move(name))
        , _data(std::move(data))
    {
        std::cout << "Parameterized constructor called\n";
    }

    
    MyClass(const MyClass& other)  /* copy constructor */
        : _id(other._id)
        , _name(other._name)
        , _data(other._data)
    {
        std::cout << "Copy constructor called\n";
    }

    
    MyClass(MyClass&& other) noexcept  /* move constructor */
        : _id(other._id)
        , _name(std::move(other._name))
        , _data(std::move(other._data))
    {
        std::cout << "Move constructor called\n";
        other._id = 0;
        other._name = "Moved";
    }

    
    MyClass& operator=(const MyClass& other)  /* copy assignment operator */
    {
        if (this != &other) {
            _id = other._id;
            _name = other._name;
            _data = other._data;
            std::cout << "Copy assignment operator called\n";
        }
        return *this;
    }

    
    MyClass& operator=(MyClass&& other) noexcept  /* move assignment operator */
    {
        if (this != &other) {
            _id = other._id;
            _name = std::move(other._name);
            _data = std::move(other._data);
            other._id = 0;
            other._name = "Moved";
            std::cout << "Move assignment operator called\n";
        }
        return *this;
    }

    ~MyClass() {
        std::cout << "Destructor called for id: " << _id << "\n";
    }

    
    void display() const  /* helper function to display the contents of the object */
    {
        std::cout << "ID: " << _id << ", Name: " << _name << ", Data: [";
        for (const auto& d : _data)
            std::cout << d << " ";
        std::cout << "]\n";
    }

    bool operator<(const MyClass& other) const  /* comparison operator for std::set */
    {
        return _id < other._id;  /* comparing by ID only */
    }
};

int main()
{
    std::cout << "start\n";
    MyClass obj1(1, "Object1", { 1, 2, 3 });
    std::cout << "--- obj1 created\n";

    MyClass obj2(2, "Object2", { 4, 5, 6 });
    std::cout << "--- obj2 created\n";

    std::set<MyClass> mySet;

    mySet.insert(obj1);  /* insertion with copying */
    std::cout << "--- obj1 inserted into the set\n";

    mySet.insert(std::move(obj2));  /* insertion with move */
    std::cout << "--- obj2 moved into the set\n";

    MyClass obj3 = obj1;  /* make a copy of an object */
    std::cout << "--- obj1 copyed to obj3\n";

    MyClass obj4 = std::move(obj3);  /* move an object */
    std::cout << "--- obj3 moved to obj4\n";

    // print all objects stored in the set
    std::cout << "\nContents of the set:\n";
    for (const auto& obj : mySet) {
        obj.display();
    }

    return 0;
}
