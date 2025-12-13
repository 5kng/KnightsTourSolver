#pragma once

#include <chrono>
#include <functional>

/**
 * @brief High-precision timer for performance measurement
 *
 * RAII-based timer that automatically measures elapsed time.
 * Supports microsecond precision using std::chrono.
 */
class Timer {
public:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using Microseconds = std::chrono::microseconds;
    using Milliseconds = std::chrono::milliseconds;

    /**
     * @brief Construct timer and start measurement
     */
    Timer() : start_(Clock::now()) {}

    /**
     * @brief Restart the timer
     */
    void reset() {
        start_ = Clock::now();
    }

    /**
     * @brief Get elapsed time in microseconds
     * @return Elapsed microseconds since construction or last reset
     */
    [[nodiscard]] long long elapsedMicroseconds() const {
        auto end = Clock::now();
        return std::chrono::duration_cast<Microseconds>(end - start_).count();
    }

    /**
     * @brief Get elapsed time in milliseconds
     * @return Elapsed milliseconds since construction or last reset
     */
    [[nodiscard]] double elapsedMilliseconds() const {
        return elapsedMicroseconds() / 1000.0;
    }

    /**
     * @brief Get elapsed time in seconds
     * @return Elapsed seconds since construction or last reset
     */
    [[nodiscard]] double elapsedSeconds() const {
        return elapsedMicroseconds() / 1000000.0;
    }

private:
    TimePoint start_;
};

/**
 * @brief Measure execution time of a function
 * @param func Function to measure
 * @return Elapsed microseconds
 */
template<typename Func>
[[nodiscard]] long long measureTime(Func&& func) {
    Timer timer;
    func();
    return timer.elapsedMicroseconds();
}

/**
 * @brief Measure execution time of a function with return value
 * @param func Function to measure
 * @return Pair of (result, elapsed microseconds)
 */
template<typename Func>
[[nodiscard]] auto measureTimeWithResult(Func&& func) {
    Timer timer;
    auto result = func();
    auto elapsed = timer.elapsedMicroseconds();
    return std::make_pair(result, elapsed);
}
