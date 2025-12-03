#include "Board.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

Board::Board(size_t width, size_t height)
    : width_(width)
    , height_(height)
    , board_(width * height, 0)
{
    if (width == 0 || height == 0) {
        throw std::invalid_argument("Board dimensions must be positive");
    }
    if (width > 1000 || height > 1000) {
        throw std::invalid_argument("Board dimensions too large (max 1000x1000)");
    }
}

bool Board::isValid(int row, int col) const noexcept {
    return row >= 0 && row < static_cast<int>(height_) &&
           col >= 0 && col < static_cast<int>(width_);
}

size_t Board::toIndex(int row, int col) const noexcept {
    return static_cast<size_t>(row) * width_ + static_cast<size_t>(col);
}

int Board::at(int row, int col) const {
    if (!isValid(row, col)) {
        throw std::out_of_range("Board coordinates out of range");
    }
    return board_[toIndex(row, col)];
}

void Board::set(int row, int col, int moveNumber) {
    if (!isValid(row, col)) {
        throw std::out_of_range("Board coordinates out of range");
    }
    board_[toIndex(row, col)] = moveNumber;
}

void Board::clear() noexcept {
    std::fill(board_.begin(), board_.end(), 0);
}

bool Board::isVisited(int row, int col) const {
    return at(row, col) != 0;
}

void Board::print() const {
    // Calculate width needed for largest move number
    int maxMoveNumber = static_cast<int>(size());
    int cellWidth = std::to_string(maxMoveNumber).length() + 1;

    std::cout << "\nBoard (" << width_ << "x" << height_ << "):\n";
    std::cout << std::string((cellWidth + 1) * width_ + 1, '-') << "\n";

    for (size_t row = 0; row < height_; ++row) {
        std::cout << "|";
        for (size_t col = 0; col < width_; ++col) {
            int value = board_[toIndex(row, col)];
            if (value == 0) {
                std::cout << std::setw(cellWidth) << "." << "|";
            } else {
                std::cout << std::setw(cellWidth) << value << "|";
            }
        }
        std::cout << "\n";
    }
    std::cout << std::string((cellWidth + 1) * width_ + 1, '-') << "\n";
}

std::vector<Move> Board::getValidMoves(int row, int col, bool onlyUnvisited) const {
    std::vector<Move> validMoves;
    validMoves.reserve(8);  // Maximum possible knight moves

    for (const auto& move : KNIGHT_MOVES) {
        int newRow = row + move.row;
        int newCol = col + move.col;

        if (isValid(newRow, newCol)) {
            if (!onlyUnvisited || !isVisited(newRow, newCol)) {
                validMoves.push_back({newRow, newCol});
            }
        }
    }

    return validMoves;
}

int Board::countValidMoves(int row, int col) const {
    int count = 0;

    for (const auto& move : KNIGHT_MOVES) {
        int newRow = row + move.row;
        int newCol = col + move.col;

        if (isValid(newRow, newCol) && !isVisited(newRow, newCol)) {
            ++count;
        }
    }

    return count;
}
