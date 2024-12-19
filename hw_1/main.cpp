
#include <iostream>
#include <iomanip>
#include <list>
#include <set>
#include <sstream>
#include <vector>
#include <string>

template <typename Container>
void printContainer(const Container& container) {

    std::ostringstream output;
    for (const auto& element : container) {
        output << element << " ";
    }

    output << std::endl;

    // Print the entire result with a single cout call.
    // This approach is thread-safe and ensures that the output string
    // will not be interrupted by other threads, as the standard guarantees
    // atomicity for individual << operations on std::cout [C++17 27.4.1.3.2]
    std::cout << output.str();
}

template <typename Container>
void printAsHex(const Container& container) {
    constexpr size_t bytesPerLine = 16;  // number of bytes per line
    constexpr size_t blockSize = 8;  // block size for splitting

    size_t offset = 0;
    std::ostringstream output;

    for (auto it = container.cbegin(); it != container.cend(); offset += bytesPerLine) {

        // print the offset
        output << std::setw(8) << std::setfill('0') << std::hex << offset << ": ";

        // print the hex representation with blocks of 8 bytes
        auto lineIt = it;
        for (size_t i = 0; i < bytesPerLine && lineIt != container.cend(); ++i, ++lineIt) {
            if (i > 0 && i % blockSize == 0) {
                output << "| ";  // add a space every blockSize bytes
            }
            output << std::setw(2) << std::setfill('0') << std::hex
                   << static_cast<int>(static_cast<unsigned char>(*lineIt)) << " ";
        }

        // fill spaces for alignment if the line is shorter
        for (size_t i = std::distance(it, lineIt); i < bytesPerLine; ++i) {
            if (i > 0 && i % blockSize == 0) {
                output << " "; // maintain block alignment
            }
            output << "   ";
        }

        // print ASCII representation
        output << " ";
        lineIt = it;
        for (size_t i = 0; i < bytesPerLine && lineIt != container.cend(); ++i, ++lineIt) {
            char c = static_cast<char>(*lineIt);
            output << (std::isprint(static_cast<unsigned char>(c)) ? c : '.');
        }

        output << "\n";
        it = lineIt;
    }

    // Print the entire result with a single cout call.
    // This approach is thread-safe and ensures that the output string
    // will not be interrupted by other threads, as the standard guarantees
    // atomicity for individual << operations on std::cout [C++17 27.4.1.3.2]
    std::cout << output.str();
}

int main() {

    // Part 1: Mandatory Task
    // Demonstrates the generic printContainer function with different STL containers.
    // It works seamlessly for containers like vector, list, and set, as long as the
    // elements are printable using the stream insertion operator (operator<<).

    std::vector<int> vec = { 1, 2, 3, 4, 5 };
    std::list<std::string> lst = { "hello", "world" };
    std::set<char> st = { 'A', 'B', 'C' };

    printContainer(vec);
    printContainer(lst);
    printContainer(st);

    // Part 2: Advanced Task (*)
    // Adds a feature to print the contents of a container as a hex dump, mimicking
    // the style of a FAR hex view. Each byte is represented in hexadecimal, and the
    // ASCII representation is displayed for printable characters.

    std::vector<uint8_t> data = {
        0xEF, 0xBB, 0xBF, 0x2E, 0x4C, 0x61, 0x6E, 0x67, 0x75, 0x61, 0x67, 0x65,
        0x3D, 0x53, 0x6C, 0x6F, 0x76, 0x61, 0x6B, 0x2C, 0x53, 0x6C, 0x6F, 0x76,
        0xEF, 0xBB, 0xBF, 0x2E, 0x4C, 0x61, 0x6E, 0x67, 0x75, 0x61, 0x67, 0x65,
        0x3D, 0x53, 0x6C, 0x6F, 0x76, 0x61, 0x6B, 0x2C, 0x53, 0x6C, 0x6F, 0x76
    };

    std::string text = "Hello, world!";

    std::cout << "\nHex dump of vector:\n";
    printAsHex(data);

    std::cout << "\nHex dump of string:\n";
    printAsHex(text);

    return 0;
}
