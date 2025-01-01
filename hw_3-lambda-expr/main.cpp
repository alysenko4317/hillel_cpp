
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> container = { 1, 2, 3, 4, 5 };

    auto increment = [&container](int value) {
        std::for_each(container.begin(), container.end(), [value](int& element) {
            element += value;
        });
    };

    increment(3);

    for (const auto& element : container) {
        std::cout << element << " ";
    }

    return 0;
}
