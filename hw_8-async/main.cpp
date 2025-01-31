
#include <iostream>
#include <vector>
#include <future>
#include <numeric>

// calculate the sum of all elements in a vector
long long calculateSum(const std::vector<int>& numbers) {
    return std::accumulate(numbers.begin(), numbers.end(), 0LL);
}

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Launch a separate thread to calculate the sum using std::async
    std::future<long long> resultFuture = std::async(std::launch::async, calculateSum, std::ref(numbers));

    // Wait for the result and retrieve it
    long long result = resultFuture.get();

    // Output the result to the console
    std::cout << "The sum of the vector elements is: " << result << std::endl;

    return 0;
}
