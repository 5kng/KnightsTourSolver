#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include "Board.h"
#include "Solver.h"

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

/**
 * @brief Result of a single benchmark run
 */
struct BenchmarkResult {
    std::string name;           // Benchmark name/description
    size_t boardSize;           // Board dimension (e.g., 8 for 8x8)
    TourType tourType;          // OPEN or CLOSED
    Statistics timing;          // Timing statistics (in microseconds)
    double successRate;         // Percentage of successful solves
    size_t totalRuns;           // Number of benchmark iterations
};

/**
 * @brief Suite for running systematic performance benchmarks
 */
class BenchmarkSuite {
public:
    /**
     * @brief Construct benchmark suite with default parameters
     * @param iterations Number of runs per configuration (default 1000)
     * @param warmupRuns Number of warmup runs to eliminate cache effects (default 10)
     */
    explicit BenchmarkSuite(size_t iterations = 1000, size_t warmupRuns = 10)
        : iterations_(iterations)
        , warmupRuns_(warmupRuns)
        , verbose_(false)
    {}

    /**
     * @brief Enable/disable verbose progress reporting
     * @param enabled true to show progress during benchmarks
     */
    void setVerbose(bool enabled) {
        verbose_ = enabled;
    }

    /**
     * @brief Benchmark a specific board size and tour type
     * @param boardSize Board dimension (e.g., 8 for 8x8)
     * @param tourType Type of tour to find (OPEN or CLOSED)
     * @param startRow Starting row (default 0)
     * @param startCol Starting column (default 0)
     * @return Benchmark results with timing statistics
     */
    [[nodiscard]] BenchmarkResult run(size_t boardSize, TourType tourType = TourType::OPEN,
                                      int startRow = 0, int startCol = 0) {
        std::string name = std::to_string(boardSize) + "x" + std::to_string(boardSize);
        if (tourType == TourType::CLOSED) {
            name += " (closed)";
        }

        if (verbose_) {
            std::cout << "Running benchmark: " << name << " [" << iterations_ << " iterations]\n";
        }

        // Warmup runs
        if (warmupRuns_ > 0) {
            for (size_t i = 0; i < warmupRuns_; ++i) {
                Board warmupBoard(boardSize, boardSize);
                Solver warmupSolver(warmupBoard);
                warmupSolver.solve(startRow, startCol, tourType);
            }
        }

        // Actual benchmark runs
        std::vector<double> times;
        times.reserve(iterations_);
        size_t successes = 0;

        for (size_t i = 0; i < iterations_; ++i) {
            Board board(boardSize, boardSize);
            Solver solver(board);

            Timer timer;
            bool solved = solver.solve(startRow, startCol, tourType);
            long long elapsed = timer.elapsedMicroseconds();

            times.push_back(static_cast<double>(elapsed));
            if (solved) {
                ++successes;
            }

            // Progress reporting
            if (verbose_ && (i + 1) % 100 == 0) {
                std::cout << "  Progress: " << (i + 1) << "/" << iterations_ << "\n";
            }
        }

        BenchmarkResult result;
        result.name = name;
        result.boardSize = boardSize;
        result.tourType = tourType;
        result.timing = Statistics::compute(times);
        result.successRate = (static_cast<double>(successes) / iterations_) * 100.0;
        result.totalRuns = iterations_;

        if (verbose_) {
            std::cout << "  Completed: " << name << "\n";
            std::cout << "  Success rate: " << result.successRate << "%\n";
            std::cout << "  Median time: " << result.timing.median << " μs\n\n";
        }

        return result;
    }

    /**
     * @brief Run benchmarks across multiple board sizes
     * @param sizes Vector of board sizes to test
     * @param tourType Type of tour to find
     * @return Vector of benchmark results
     */
    [[nodiscard]] std::vector<BenchmarkResult> runMultiple(
        const std::vector<size_t>& sizes,
        TourType tourType = TourType::OPEN
    ) {
        std::vector<BenchmarkResult> results;
        results.reserve(sizes.size());

        for (size_t size : sizes) {
            results.push_back(run(size, tourType));
        }

        return results;
    }

private:
    size_t iterations_;
    size_t warmupRuns_;
    bool verbose_;
};

/**
 * @brief Print benchmark results in a formatted table
 * @param results Vector of benchmark results to display
 */
inline void printResults(const std::vector<BenchmarkResult>& results) {
    if (results.empty()) {
        std::cout << "No benchmark results to display.\n";
        return;
    }

    std::cout << "\n=== Benchmark Results ===\n\n";
    
    // Header
    std::cout << std::left
              << std::setw(20) << "Configuration"
              << std::setw(12) << "Mean (μs)"
              << std::setw(12) << "Median (μs)"
              << std::setw(12) << "StdDev (μs)"
              << std::setw(12) << "Min (μs)"
              << std::setw(12) << "Max (μs)"
              << std::setw(12) << "P95 (μs)"
              << std::setw(12) << "P99 (μs)"
              << std::setw(12) << "Success %"
              << std::setw(10) << "Runs"
              << "\n";
    
    std::cout << std::string(130, '-') << "\n";
    
    // Data rows
    for (const auto& result : results) {
        std::cout << std::left << std::fixed << std::setprecision(2)
                  << std::setw(20) << result.name
                  << std::setw(12) << result.timing.mean
                  << std::setw(12) << result.timing.median
                  << std::setw(12) << result.timing.stdDev
                  << std::setw(12) << result.timing.min
                  << std::setw(12) << result.timing.max
                  << std::setw(12) << result.timing.p95
                  << std::setw(12) << result.timing.p99
                  << std::setw(12) << result.successRate
                  << std::setw(10) << result.totalRuns
                  << "\n";
    }
    
    std::cout << "\n";
}

/**
 * @brief Export benchmark results to CSV file
 * @param results Vector of benchmark results to export
 * @param filename Output filename
 * @return true if export successful, false otherwise
 */
inline bool exportToCSV(const std::vector<BenchmarkResult>& results, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "' for writing.\n";
        return false;
    }

    // CSV Header
    file << "Configuration,BoardSize,TourType,Mean_us,Median_us,StdDev_us,Min_us,Max_us,"
         << "P95_us,P99_us,Range_us,Variance,SuccessRate,Runs\n";

    // Data rows
    for (const auto& result : results) {
        file << result.name << ","
             << result.boardSize << ","
             << (result.tourType == TourType::OPEN ? "OPEN" : "CLOSED") << ","
             << result.timing.mean << ","
             << result.timing.median << ","
             << result.timing.stdDev << ","
             << result.timing.min << ","
             << result.timing.max << ","
             << result.timing.p95 << ","
             << result.timing.p99 << ","
             << result.timing.range << ","
             << result.timing.variance << ","
             << result.successRate << ","
             << result.totalRuns << "\n";
    }

    file.close();
    std::cout << "Results exported to '" << filename << "'\n";
    return true;
}

/**
 * @brief Print a single benchmark result with detailed statistics
 * @param result Benchmark result to display
 */
inline void printDetailedResult(const BenchmarkResult& result) {
    std::cout << "\n=== " << result.name << " ===\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Board size:    " << result.boardSize << "x" << result.boardSize << "\n";
    std::cout << "Tour type:     " << (result.tourType == TourType::OPEN ? "OPEN" : "CLOSED") << "\n";
    std::cout << "Runs:          " << result.totalRuns << "\n";
    std::cout << "Success rate:  " << result.successRate << "%\n\n";
    
    std::cout << "Timing Statistics (microseconds):\n";
    std::cout << "  Mean:        " << result.timing.mean << " μs\n";
    std::cout << "  Median:      " << result.timing.median << " μs\n";
    std::cout << "  Std Dev:     " << result.timing.stdDev << " μs\n";
    std::cout << "  Min:         " << result.timing.min << " μs\n";
    std::cout << "  Max:         " << result.timing.max << " μs\n";
    std::cout << "  Range:       " << result.timing.range << " μs\n";
    std::cout << "  P95:         " << result.timing.p95 << " μs\n";
    std::cout << "  P99:         " << result.timing.p99 << " μs\n";
    std::cout << "\n";
    
    // Convert to milliseconds if > 1000 μs
    if (result.timing.median > 1000.0) {
        std::cout << "  Median:      " << (result.timing.median / 1000.0) << " ms\n";
    }
}
