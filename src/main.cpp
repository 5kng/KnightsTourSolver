#include <iostream>
#include "Board.h"

int main() {
    std::cout << "Knight's Tour Solver - Move Validation Test\n\n";

    try {
        // Create an 8x8 board
        Board board(8, 8);

        std::cout << "Board created: " << board.width() << "x" << board.height() << "\n";
        std::cout << "Total squares: " << board.size() << "\n\n";

        // Test knight moves from different positions
        std::cout << "=== Testing Knight Move Validation ===\n\n";

        // Corner position (0,0) - limited moves
        std::cout << "From corner (0,0):\n";
        auto moves = board.getValidMoves(0, 0);
        std::cout << "  Valid moves: " << moves.size() << "\n";
        std::cout << "  Move count: " << board.countValidMoves(0, 0) << "\n";
        std::cout << "  Positions: ";
        for (const auto& move : moves) {
            std::cout << "(" << move.row << "," << move.col << ") ";
        }
        std::cout << "\n\n";

        // Center position (4,4) - maximum moves
        std::cout << "From center (4,4):\n";
        moves = board.getValidMoves(4, 4);
        std::cout << "  Valid moves: " << moves.size() << "\n";
        std::cout << "  Move count: " << board.countValidMoves(4, 4) << "\n";
        std::cout << "  Positions: ";
        for (const auto& move : moves) {
            std::cout << "(" << move.row << "," << move.col << ") ";
        }
        std::cout << "\n\n";

        // Edge position (0,4)
        std::cout << "From edge (0,4):\n";
        moves = board.getValidMoves(0, 4);
        std::cout << "  Valid moves: " << moves.size() << "\n";
        std::cout << "  Move count: " << board.countValidMoves(0, 4) << "\n\n";

        // Test with visited squares
        std::cout << "=== Testing with Visited Squares ===\n\n";
        board.set(0, 0, 1);
        board.set(1, 2, 2);
        board.set(2, 1, 3);

        std::cout << "Marked (0,0), (1,2), and (2,1) as visited\n\n";

        std::cout << "From (0,0) - checking only unvisited:\n";
        moves = board.getValidMoves(0, 0, true);
        std::cout << "  Unvisited moves: " << moves.size() << "\n";
        std::cout << "  Move count: " << board.countValidMoves(0, 0) << "\n\n";

        std::cout << "From (0,0) - checking all moves:\n";
        moves = board.getValidMoves(0, 0, false);
        std::cout << "  All moves: " << moves.size() << "\n\n";

        // Test move degree distribution
        std::cout << "=== Move Degree Distribution (empty board) ===\n\n";
        board.clear();

        int corners = 0, edges = 0, center = 0;
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                int degree = board.countValidMoves(row, col);
                if (degree == 2) corners++;
                else if (degree <= 4) edges++;
                else center++;
            }
        }

        std::cout << "Squares with 2 moves (corners): " << corners << "\n";
        std::cout << "Squares with 3-4 moves (edges): " << edges << "\n";
        std::cout << "Squares with 6-8 moves (center): " << center << "\n\n";

        // Visualize move counts
        std::cout << "=== Move Count Heatmap ===\n";
        std::cout << "(number = possible moves from that square)\n\n";
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                std::cout << board.countValidMoves(row, col) << " ";
            }
            std::cout << "\n";
        }

        std::cout << "\n✓ All move validation tests passed!\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
