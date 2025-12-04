#pragma once

#include "Board.h"
#include <vector>

/**
 * @brief Type of tour to find
 */
enum class TourType {
    OPEN,    // Knight can end anywhere
    CLOSED   // Knight must end one move from start (forms a cycle)
};

/**
 * @brief Solves the Knight's Tour problem using backtracking
 *
 * This solver uses naive backtracking without heuristics.
 * It systematically explores all possible move sequences until
 * a solution is found or all possibilities are exhausted.
 */
class Solver {
public:
    /**
     * @brief Construct a solver for the given board
     * @param board Reference to the board to solve
     */
    explicit Solver(Board& board);

    /**
     * @brief Solve the Knight's Tour problem
     * @param startRow Starting row position (default 0)
     * @param startCol Starting column position (default 0)
     * @param type Tour type: OPEN or CLOSED (default OPEN)
     * @return true if solution found, false otherwise
     */
    bool solve(int startRow = 0, int startCol = 0, TourType type = TourType::OPEN);

    /**
     * @brief Get the solution path (sequence of moves)
     * @return Vector of moves representing the solution
     */
    [[nodiscard]] const std::vector<Move>& getPath() const { return path_; }

    /**
     * @brief Get number of backtracks performed during solve
     * @return Total number of times the algorithm backtracked
     */
    [[nodiscard]] size_t getBacktrackCount() const { return backtrackCount_; }

    /**
     * @brief Reset solver state
     */
    void reset();

private:
    Board& board_;
    std::vector<Move> path_;
    size_t backtrackCount_;
    int startRow_;
    int startCol_;
    TourType tourType_;

    /**
     * @brief Recursive backtracking function
     * @param row Current row position
     * @param col Current column position
     * @param moveNumber Current move number (1-indexed)
     * @return true if solution found from this position
     */
    bool backtrack(int row, int col, int moveNumber);

    /**
     * @brief Check if current state is a valid solution
     * @param moveNumber Current move number
     * @return true if all squares visited (and closed tour requirements met)
     */
    [[nodiscard]] bool isSolution(int moveNumber) const;
};
