#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <limits>
#include <numeric>
#include <vector>

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

/**
 * @brief Statistical summary of performance measurements
 */
struct Statistics {
    double mean;           // Average value
    double median;         // Middle value (50th percentile)
    double stdDev;         // Standard deviation
    double min;            // Minimum value
    double max;            // Maximum value
    double p95;            // 95th percentile
    double p99;            // 99th percentile
    size_t count;          // Number of samples
    double range;          // max - min
    double variance;       // Variance (stdDev^2)

    /**
     * @brief Compute statistics from a vector of values
     * @param values Data samples (will be sorted in-place)
     * @return Statistics structure with computed values
     */
    [[nodiscard]] static Statistics compute(std::vector<double>& values) {
        if (values.empty()) {
            return {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        }

        Statistics stats;
        stats.count = values.size();

        // Sort for percentile calculations
        std::sort(values.begin(), values.end());

        // Min and max
        stats.min = values.front();
        stats.max = values.back();
        stats.range = stats.max - stats.min;

        // Mean
        double sum = std::accumulate(values.begin(), values.end(), 0.0);
        stats.mean = sum / static_cast<double>(values.size());

        // Median (50th percentile)
        stats.median = percentile(values, 50.0);

        // Percentiles
        stats.p95 = percentile(values, 95.0);
        stats.p99 = percentile(values, 99.0);

        // Variance and standard deviation
        double sumSquaredDiff = 0.0;
        for (double value : values) {
            double diff = value - stats.mean;
            sumSquaredDiff += diff * diff;
        }
        stats.variance = sumSquaredDiff / static_cast<double>(values.size());
        stats.stdDev = std::sqrt(stats.variance);

        return stats;
    }

private:
    /**
     * @brief Calculate percentile from sorted data
     * @param sortedValues Vector of sorted values
     * @param p Percentile (0-100)
     * @return Value at the given percentile
     */
    [[nodiscard]] static double percentile(const std::vector<double>& sortedValues, double p) {
        if (sortedValues.empty()) {
            return 0.0;
        }
        if (sortedValues.size() == 1) {
            return sortedValues[0];
        }

        double index = (p / 100.0) * (sortedValues.size() - 1);
        size_t lower = static_cast<size_t>(std::floor(index));
        size_t upper = static_cast<size_t>(std::ceil(index));

        if (lower == upper) {
            return sortedValues[lower];
        }

        // Linear interpolation between lower and upper
        double weight = index - lower;
        return sortedValues[lower] * (1.0 - weight) + sortedValues[upper] * weight;
    }
};
