#include "collatzcalculator.h"

#include <QtConcurrent>
#include <QFuture>
#include <QElapsedTimer>
#include <QList>
#include <limits>
#include <stdexcept>

// Function to compute the length of the Collatz sequence for a single number.
// Throws std::overflow_error if an overflow occurs during the calculation.
static quint64 collatzLength(quint64 start) {
    quint64 length = 1;
    quint64 n = start;
    while (n != 1) {
        if ((n & 1ULL) == 0ULL) {
            n >>= 1; // Efficient division by 2
        } else {
            // Check for overflow before computing 3*n + 1
            if (n > (std::numeric_limits<quint64>::max() - 1) / 3) {
                throw std::overflow_error("64-bit integer overflow during calculation");
            }
            n = 3 * n + 1;
        }
        length++;
    }
    return length;
}

// Structure to store intermediate results in a subrange
struct RangeResult {
    quint64 bestNumber;
    quint64 bestLength;
};

// Function that processes the range [start, end] and finds the number with the longest Collatz sequence.
// If stopFlag is set, the processing is terminated early.
static RangeResult processRange(quint64 start, quint64 end, std::atomic_bool &stopFlag) {
    RangeResult result { 0, 0 };
    for (quint64 i = start; i <= end; ++i) {
        if (stopFlag.load()) {
            break;
        }
        quint64 length = collatzLength(i);
        if (length > result.bestLength) {
            result.bestLength = length;
            result.bestNumber = i;
        }
    }
    return result;
}

CollatzResult CollatzCalculator::calculate(quint64 limit, int numThreads, std::atomic_bool &stopFlag) {
    QElapsedTimer timer;
    timer.start();

    QList<QFuture<RangeResult>> futures;

    // Divide the range [1, limit] into approximately equal parts
    quint64 chunkSize = limit / numThreads;
    if (chunkSize == 0) {
        chunkSize = 1;
    }
    quint64 currentStart = 1;
    for (int i = 0; i < numThreads && currentStart <= limit; ++i) {
        quint64 currentEnd = (i == numThreads - 1) ? limit : (currentStart + chunkSize - 1);
        QFuture<RangeResult> future = QtConcurrent::run(processRange, currentStart, currentEnd, std::ref(stopFlag));
        futures.append(future);
        currentStart = currentEnd + 1;
    }

    // Gather results from each subrange
    RangeResult globalResult { 0, 0 };
    for (auto &future : futures) {
        future.waitForFinished();
        RangeResult localResult = future.result();
        if (localResult.bestLength > globalResult.bestLength) {
            globalResult = localResult;
        }
    }

    qint64 elapsed = timer.elapsed();
    CollatzResult result;
    result.bestNumber = globalResult.bestNumber;
    result.bestLength = globalResult.bestLength;
    result.timeMs = elapsed;
    return result;
}
