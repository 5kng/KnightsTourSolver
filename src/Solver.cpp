#include "Solver.h"
#include <algorithm>

Solver::Solver(Board& board)
    : board_(board)
    , backtrackCount_(0)
    , startRow_(0)
    , startCol_(0)
    , tourType_(TourType::OPEN)
{
    path_.reserve(board.size());
}

void Solver::reset() {
    board_.clear();
    path_.clear();
    backtrackCount_ = 0;
}

bool Solver::solve(int startRow, int startCol, TourType type) {
    // Validate starting position
    if (!board_.isValid(startRow, startCol)) {
        return false;
    }

    // Reset state
    board_.clear();
    path_.clear();
    backtrackCount_ = 0;
    startRow_ = startRow;
    startCol_ = startCol;
    tourType_ = type;

    // Place the knight at starting position
    board_.set(startRow, startCol, 1);
    path_.push_back({startRow, startCol});

    // Start backtracking from move 2
    return backtrack(startRow, startCol, 2);
}

bool Solver::backtrack(int row, int col, int moveNumber) {
    // Check if we've visited all squares
    if (isSolution(moveNumber)) {
        return true;
    }

    // Get all valid unvisited moves from current position
    auto validMoves = board_.getValidMoves(row, col, true);

    // Try each valid move
    for (const auto& move : validMoves) {
        // Make move
        board_.set(move.row, move.col, moveNumber);
        path_.push_back(move);

        // Recursive call: try to solve from this new position
        if (backtrack(move.row, move.col, moveNumber + 1)) {
            return true;  // Solution found!
        }

        // Undo move (backtrack)
        board_.set(move.row, move.col, 0);
        path_.pop_back();
        ++backtrackCount_;
    }

    // No solution found from this position
    return false;
}

bool Solver::isSolution(int moveNumber) const {
    // Have we visited all squares?
    if (moveNumber != static_cast<int>(board_.size()) + 1) {
        return false;
    }

    // For open tour, we're done
    if (tourType_ == TourType::OPEN) {
        return true;
    }

    // For closed tour, verify we can return to starting position
    const auto& lastMove = path_.back();
    auto validMoves = board_.getValidMoves(lastMove.row, lastMove.col, false);

    // Check if starting position is reachable from final position
    return std::any_of(validMoves.begin(), validMoves.end(),
        [this](const Move& m) {
            return m.row == startRow_ && m.col == startCol_;
        });
}
