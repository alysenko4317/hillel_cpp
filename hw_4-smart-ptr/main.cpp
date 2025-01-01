
#include "my_unique_ptr.h"
#include <iostream>

int main() {
    // single object
    MyUniquePtr<int> singlePtr(new int(42));
    std::cout << "Single object value: " << *singlePtr << "\n";

    // transfer ownership using move constructor
    MyUniquePtr<int> movedPtr(std::move(singlePtr));
    if (!singlePtr.Get()) {
        std::cout << "singlePtr is now nullptr after move.\n";
    }
    std::cout << "Moved object value: " << *movedPtr << "\n";

    // an array oriented variant
    MyUniquePtr<int[]> arrayPtr(new int[5] {1, 2, 3, 4, 5});
    for (std::size_t i = 0; i < 5; ++i) {
        std::cout << "Array element [" << i << "]: " << arrayPtr[i] << "\n";
    }

    // releasing ownership of the array
    int* rawArray = arrayPtr.Release();
    if (!arrayPtr.Get()) {
        std::cout << "arrayPtr is now nullptr after release.\n";
    }

    // clean up the raw pointer manually
    delete[] rawArray;

    return 0;
}
