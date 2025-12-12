#pragma once

#include <vector>
#include <cstddef>
#include <stdexcept>

/**
 * @brief Represents a position on the board
 */
struct Move {
    int row;
    int col;
};

/**
 * @brief Represents a chessboard for the Knight's Tour problem
 *
 * The board uses a 1D vector for efficient memory layout and cache performance.
 * Each square stores the move number (1-indexed), with 0 indicating unvisited.
 */
class Board {
public:
    /**
     * @brief Construct a board of given dimensions
     * @param width Board width (number of columns)
     * @param height Board height (number of rows)
     * @throws std::invalid_argument if dimensions are invalid
     */
    explicit Board(size_t width = 8, size_t height = 8);

    /**
     * @brief Get board width
     * @return Number of columns
     */
    [[nodiscard]] size_t width() const noexcept { return width_; }

    /**
     * @brief Get board height
     * @return Number of rows
     */
    [[nodiscard]] size_t height() const noexcept { return height_; }

    /**
     * @brief Get total number of squares
     * @return Total squares on the board
     */
    [[nodiscard]] size_t size() const noexcept { return width_ * height_; }

    /**
     * @brief Check if coordinates are within board bounds
     * @param row Row coordinate
     * @param col Column coordinate
     * @return true if coordinates are valid
     */
    [[nodiscard]] bool isValid(int row, int col) const noexcept;

    /**
     * @brief Get move number at position (0 = unvisited)
     * @param row Row coordinate
     * @param col Column coordinate
     * @return Move number at this position
     * @throws std::out_of_range if coordinates are invalid
     */
    [[nodiscard]] int at(int row, int col) const;

    /**
     * @brief Set move number at position
     * @param row Row coordinate
     * @param col Column coordinate
     * @param moveNumber Move number to set (0 = unvisited)
     * @throws std::out_of_range if coordinates are invalid
     */
    void set(int row, int col, int moveNumber);

    /**
     * @brief Clear the board (reset all squares to unvisited)
     */
    void clear() noexcept;

    /**
     * @brief Check if position has been visited
     * @param row Row coordinate
     * @param col Column coordinate
     * @return true if square has been visited
     */
    [[nodiscard]] bool isVisited(int row, int col) const;

    /**
     * @brief Print the board to stdout (for debugging)
     */
    void print() const;

    /**
     * @brief Print the board with row/column labels and optional highlights
     * @param highlightStart Optional starting position to highlight
     * @param highlightEnd Optional ending position to highlight
     */
    void printDetailed(const Move* highlightStart = nullptr, const Move* highlightEnd = nullptr) const;

    /**
     * @brief Print a compact representation (for large boards)
     * @param maxSize Maximum dimension to print at full size
     */
    void printCompact(size_t maxSize = 12) const;

    /**
     * @brief Get all valid knight moves from a position
     * @param row Current row
     * @param col Current column
     * @param onlyUnvisited If true, only return unvisited squares
     * @return Vector of valid moves
     */
    [[nodiscard]] std::vector<Move> getValidMoves(int row, int col, bool onlyUnvisited = true) const;

    /**
     * @brief Count number of valid knight moves from a position
     * @param row Current row
     * @param col Current column
     * @return Number of valid unvisited moves (degree in graph theory)
     */
    [[nodiscard]] int countValidMoves(int row, int col) const;

private:
    // Knight move offsets (L-shaped: 2 squares in one direction, 1 in perpendicular)
    static constexpr Move KNIGHT_MOVES[8] = {
        {-2, -1}, {-2, +1},  // Up-left, up-right
        {-1, -2}, {-1, +2},  // Left-up, right-up
        {+1, -2}, {+1, +2},  // Left-down, right-down
        {+2, -1}, {+2, +1}   // Down-left, down-right
    };
    size_t width_;
    size_t height_;
    std::vector<int> board_;

    /**
     * @brief Convert 2D coordinates to 1D index
     * @param row Row coordinate
     * @param col Column coordinate
     * @return 1D index in the board vector
     */
    [[nodiscard]] size_t toIndex(int row, int col) const noexcept;
};
