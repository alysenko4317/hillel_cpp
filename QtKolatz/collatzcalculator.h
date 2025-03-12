#ifndef COLLATZCALCULATOR_H
#define COLLATZCALCULATOR_H

#include <QtGlobal>
#include <atomic>
#include <stdexcept>
#include <QString>

// Structure to store the full calculation result for a range.
struct CollatzResult {
    quint64 bestNumber;  // Number with the longest chain.
    quint64 bestLength;  // Length of that chain.
    qint64 timeMs;       // Total calculation time in milliseconds.
};

// Structure to store the test result for a single starting value.
// It includes both the length and the full sequence (as a QString).
struct CollatzTestResult {
    quint64 length;      // Length of the sequence.
    QString sequence;    // The complete sequence (e.g. "13 → 40 → 20 → ... → 1").
};

class CollatzCalculator {
public:
    // Main calculation function for the range [1, limit].
    // Uses numThreads threads and a stopFlag for cancellation.
    // Throws std::overflow_error if an overflow occurs.
    static CollatzResult calculate(quint64 limit, int numThreads, std::atomic_bool &stopFlag);

    // Test function: computes the Collatz sequence for a single starting value.
    // It returns both the sequence (as a string) and its length.
    // This is intended only for test cases.
    static CollatzTestResult getTestSequence(quint64 start);
};

#endif // COLLATZCALCULATOR_H
