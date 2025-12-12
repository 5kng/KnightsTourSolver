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
 * @brief Statistics about a solution path
 */
struct PathStatistics {
    size_t totalMoves;          // Total number of moves in the path
    size_t cornerVisits;        // Number of corner squares visited
    size_t edgeVisits;          // Number of edge squares visited (excluding corners)
    size_t centerVisits;        // Number of center squares visited
    double averageDistanceFromCenter;  // Average Manhattan distance from board center
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

    /**
     * @brief Validate that the current path is a legal knight's tour
     * @return true if the path is valid, false otherwise
     */
    [[nodiscard]] bool validatePath() const;

    /**
     * @brief Get statistics about the solution path
     * @return PathStatistics structure with path metrics
     */
    [[nodiscard]] PathStatistics getPathStatistics() const;

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

    /**
     * @brief Calculate the degree of a move (number of available moves from that position)
     *
     * The degree represents how many valid unvisited squares the knight can reach
     * from the given position. This is a key metric for move ordering heuristics.
     *
     * @param move The move to calculate degree for
     * @return Number of valid unvisited moves from that position
     */
    [[nodiscard]] int calculateDegree(const Move& move) const;

    /**
     * @brief Count available moves from a given position
     *
     * Helper function to determine how many unvisited squares are accessible
     * from the specified position. Used for move ordering and heuristics.
     *
     * @param row Row position to check from
     * @param col Column position to check from
     * @return Number of valid unvisited moves available
     */
    [[nodiscard]] int countAvailableMoves(int row, int col) const;

    /**
     * @brief Sort moves using a move ordering heuristic
     *
     * Sorts moves in-place based on their desirability. Currently supports
     * ordering by degree (Warnsdorff's heuristic foundation).
     * Lower degree moves are preferred as they visit "harder to reach" squares first.
     *
     * @param moves Vector of moves to sort (modified in-place)
     */
    void sortMoves(std::vector<Move>& moves) const;

    /**
     * @brief Check if a move would create isolated squares (dead ends)
     *
     * Performs look-ahead pruning by temporarily making a move and checking
     * if any of its neighbors would become isolated (degree 0). This helps
     * avoid exploring paths that will inevitably fail.
     *
     * @param move The move to check
     * @param moveNumber The move number that would be assigned
     * @return true if the move creates dead ends, false otherwise
     */
    [[nodiscard]] bool createsDeadEnd(const Move& move, int moveNumber) const;
};
