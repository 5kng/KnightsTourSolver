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

    // Apply Warnsdorff's heuristic: sort moves by degree (ascending)
    sortMoves(validMoves);

    // Try each valid move
    for (const auto& move : validMoves) {
        // Early termination: skip moves that create dead ends
        // (unless it's our only option)
        if (validMoves.size() > 1 && createsDeadEnd(move, moveNumber)) {
            continue;  // Skip this move - it would isolate a square
        }

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

int Solver::calculateDegree(const Move& move) const {
    return countAvailableMoves(move.row, move.col);
}

int Solver::countAvailableMoves(int row, int col) const {
    // Get all valid unvisited moves from this position
    auto moves = board_.getValidMoves(row, col, true);
    return static_cast<int>(moves.size());
}

void Solver::sortMoves(std::vector<Move>& moves) const {
    // Helper function to calculate Manhattan distance from board center
    auto distanceFromCenter = [this](const Move& move) {
        int centerRow = static_cast<int>(board_.height()) / 2;
        int centerCol = static_cast<int>(board_.width()) / 2;
        return std::abs(move.row - centerRow) + std::abs(move.col - centerCol);
    };

    // Sort moves by degree (ascending order) with tie-breaking
    // Warnsdorff's heuristic: choose squares with fewest onward moves first
    // This visits "harder to reach" corners and edges early in the search
    std::sort(moves.begin(), moves.end(),
        [this, &distanceFromCenter](const Move& a, const Move& b) {
            int degreeA = calculateDegree(a);
            int degreeB = calculateDegree(b);

            // Primary criterion: prefer lower degree (Warnsdorff's rule)
            if (degreeA != degreeB) {
                return degreeA < degreeB;
            }

            // Tie-breaker: when degrees are equal, prefer moves farther from center
            // This prioritizes visiting edge/corner squares earlier
            return distanceFromCenter(a) > distanceFromCenter(b);
        });
}

bool Solver::createsDeadEnd(const Move& move, int moveNumber) const {
    // Temporarily make the move
    board_.set(move.row, move.col, moveNumber);

    // Check if any neighbor of this move would become isolated
    bool hasDeadEnd = false;
    auto neighbors = board_.getValidMoves(move.row, move.col, true);

    for (const auto& neighbor : neighbors) {
        // If this neighbor would have no valid moves after our move, it's a dead end
        if (countAvailableMoves(neighbor.row, neighbor.col) == 0) {
            hasDeadEnd = true;
            break;
        }
    }

    // Undo the temporary move
    board_.set(move.row, move.col, 0);

    return hasDeadEnd;
}

bool Solver::validatePath() const {
    // Empty path is invalid
    if (path_.empty()) {
        return false;
    }

    // Path should cover all squares
    if (path_.size() != board_.size()) {
        return false;
    }

    // Check that all moves are unique (no square visited twice)
    std::vector<bool> visited(board_.size(), false);
    for (const auto& move : path_) {
        // Check move is within bounds
        if (!board_.isValid(move.row, move.col)) {
            return false;
        }

        // Convert to index and check if already visited
        size_t idx = static_cast<size_t>(move.row) * board_.width() + static_cast<size_t>(move.col);
        if (visited[idx]) {
            return false;  // Square visited twice!
        }
        visited[idx] = true;
    }

    // Check that consecutive moves are legal knight moves
    for (size_t i = 1; i < path_.size(); ++i) {
        const auto& prev = path_[i - 1];
        const auto& curr = path_[i];

        int rowDiff = std::abs(curr.row - prev.row);
        int colDiff = std::abs(curr.col - prev.col);

        // Valid knight move: (2,1) or (1,2)
        bool isValidKnightMove = (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
        if (!isValidKnightMove) {
            return false;
        }
    }

    // If closed tour, check that last move can reach first move
    if (tourType_ == TourType::CLOSED && path_.size() > 1) {
        const auto& first = path_.front();
        const auto& last = path_.back();

        int rowDiff = std::abs(first.row - last.row);
        int colDiff = std::abs(first.col - last.col);

        bool canClose = (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
        if (!canClose) {
            return false;
        }
    }

    return true;
}

PathStatistics Solver::getPathStatistics() const {
    PathStatistics stats{};
    stats.totalMoves = path_.size();
    stats.cornerVisits = 0;
    stats.edgeVisits = 0;
    stats.centerVisits = 0;
    stats.averageDistanceFromCenter = 0.0;

    if (path_.empty()) {
        return stats;
    }

    int centerRow = static_cast<int>(board_.height()) / 2;
    int centerCol = static_cast<int>(board_.width()) / 2;
    int maxRow = static_cast<int>(board_.height()) - 1;
    int maxCol = static_cast<int>(board_.width()) - 1;

    double totalDistance = 0.0;

    for (const auto& move : path_) {
        // Check if corner
        bool isCorner = (move.row == 0 && move.col == 0) ||
                        (move.row == 0 && move.col == maxCol) ||
                        (move.row == maxRow && move.col == 0) ||
                        (move.row == maxRow && move.col == maxCol);

        // Check if edge (but not corner)
        bool isEdge = !isCorner && (move.row == 0 || move.row == maxRow ||
                                     move.col == 0 || move.col == maxCol);

        if (isCorner) {
            ++stats.cornerVisits;
        } else if (isEdge) {
            ++stats.edgeVisits;
        } else {
            ++stats.centerVisits;
        }

        // Calculate Manhattan distance from center
        totalDistance += std::abs(move.row - centerRow) + std::abs(move.col - centerCol);
    }

    stats.averageDistanceFromCenter = totalDistance / static_cast<double>(path_.size());

    return stats;
}
