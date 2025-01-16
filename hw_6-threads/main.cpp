#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>  // for std::for_each

// Function to increment elements in a specific range
void incrementRange(std::vector<int>& vec, size_t start, size_t end, int incrementValue) {
    // Increment each element in the assigned range
    for (size_t i = start; i < end; ++i) {
        vec[i] += incrementValue;
    }
}

int main() {
    // Create a vector with some initial values
    const size_t vectorSize = 100; // Total size of the vector
    std::vector<int> vec(vectorSize, 0); // Initialize all elements to 0

    int incrementValue = 5; // Value to increment each element
    size_t numThreads = 4;  // Number of threads to use

    // Determine the size of each chunk
    size_t chunkSize = (vectorSize + numThreads - 1) / numThreads; // Ensure all elements are processed

    // Vector to hold thread objects
    std::vector<std::thread> threads;

    // Launch threads
    for (size_t t = 0; t < numThreads; ++t) {
        size_t start = t * chunkSize; // Start index for this thread
        size_t end = std::min(start + chunkSize, vectorSize); // End index for this thread

        // Create and start the thread
        threads.emplace_back(incrementRange, std::ref(vec), start, end, incrementValue);
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    // Output the updated vector
    for (const auto& value : vec) {
        std::cout << value << " ";
    }

    return 0;
}
