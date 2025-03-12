#ifndef COLLATZCALCULATOR_H
#define COLLATZCALCULATOR_H

#include <QtGlobal>
#include <atomic>
#include <stdexcept>

// Structure to store calculation results
struct CollatzResult {
    quint64 bestNumber;  // Number with the longest chain
    quint64 bestLength;  // Length of the chain
    qint64 timeMs;       // Calculation time in milliseconds
};

class CollatzCalculator {
public:
    // Calculates the Collatz sequences for numbers in the range [1, limit]
    // using numThreads threads. The stopFlag allows early termination.
    // In case of overflow, throws std::overflow_error.
    static CollatzResult calculate(quint64 limit, int numThreads, std::atomic_bool &stopFlag);
};

#endif // COLLATZCALCULATOR_H
