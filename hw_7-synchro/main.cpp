#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <atomic>
#include <chrono>

// Key Points in Implementation:
// 
// Producers:
// - Each producer thread generates data, adds it to the shared buffer, and notifies
//   a consumer via the condition variable.
// - The producer stops producing when the total data limit (TOTAL_DATA) is reached.
// 
// Consumers:
// - Each consumer thread waits for data in the buffer.
// - When data is available, it retrieves and processes the data.
// - Consumers terminate when the buffer is empty, and production is marked as complete.
// 
// Synchronization:
// - A std::mutex ensures safe access to the shared buffer.
// - A std::condition_variable coordinates producers and consumers, avoiding busy waiting.
// 
// Practicality:
// - The system simulates real-world scenarios like logging systems or task queues,
//   where multiple producers generate tasks, and consumers process them.


// Number of producer threads
constexpr int NUM_PRODUCERS = 3;

// Number of consumer threads
constexpr int NUM_CONSUMERS = 2;

// Total number of data items to produce
constexpr int TOTAL_DATA = 50;

std::queue<int> buffer;                 // Shared buffer
std::mutex bufferMutex;                 // Mutex for protecting buffer access
std::condition_variable cv;             // Condition variable for synchronization
std::atomic<int> produced{ 0 };   // Total number of produced items
std::atomic<int> consumed{ 0 };   // Total number of consumed items
bool productionComplete = false;        // Flag indicating production is complete

// Producer function
void producer(int id) {
    while (true) {
        // Simulate data preparation
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        int data = ++produced;
        if (data > TOTAL_DATA) {
            break;  // Stop producing if we've reached the total data limit
        }

        {
            // Lock the buffer and add data
            std::lock_guard<std::mutex> lock(bufferMutex);
            buffer.push(data);
            std::cout << ("Producer " + std::to_string(id) + " produced data: " + std::to_string(data) + "\n");
        }

        cv.notify_one();  // Notify a consumer that data is available
    }

    std::cout << ("Producer " + std::to_string(id) + " finished work.\n");
}

// Consumer function
void consumer(int id) {
    while (true) {
        int data;
        {
            std::unique_lock<std::mutex> lock(bufferMutex);
            cv.wait(lock, [] { return !buffer.empty() || productionComplete; });

            if (buffer.empty() && productionComplete) {
                break;  // Exit if no data is left and production is complete
            }

            // Retrieve data from the buffer
            data = buffer.front();
            buffer.pop();
            consumed++;
            std::cout << ("Consumer " + std::to_string(id) + " consumed data: " + std::to_string(data) + "\n");
        }

        // Simulate data processing
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << ("Consumer " + std::to_string(id) + " finished work.\n");
}

int main() {
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    // Start producer threads
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        producers.emplace_back(producer, i + 1);
    }

    // Start consumer threads
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        consumers.emplace_back(consumer, i + 1);
    }

    // Wait for all producers to finish
    for (auto& producerThread : producers) {
        producerThread.join();
    }

    // Signal that production is complete
    {
        std::lock_guard<std::mutex> lock(bufferMutex);
        productionComplete = true;
    }
    cv.notify_all();

    // Wait for all consumers to finish
    for (auto& consumerThread : consumers) {
        consumerThread.join();
    }

    std::cout << "All data produced: " << produced.load() << ", All data consumed: " << consumed.load() << std::endl;

    return 0;
}
