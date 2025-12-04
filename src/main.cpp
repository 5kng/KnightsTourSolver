#include <iostream>
#include <chrono>
#include "Board.h"
#include "Solver.h"

int main() {
    std::cout << "Knight's Tour Solver - Backtracking Algorithm Test\n\n";

    try {
        // Test on a small 5x5 board first
        std::cout << "=== Testing on 5x5 Board ===\n\n";
        Board board5(5, 5);
        Solver solver5(board5);

        std::cout << "Attempting to solve 5x5 board starting at (0,0)...\n";

        auto start = std::chrono::high_resolution_clock::now();
        bool solved5 = solver5.solve(0, 0, TourType::OPEN);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        if (solved5) {
            std::cout << "✓ Solution found!\n";
            std::cout << "  Time: " << duration.count() << " ms\n";
            std::cout << "  Backtracks: " << solver5.getBacktrackCount() << "\n";
            std::cout << "  Moves: " << solver5.getPath().size() << "\n\n";

            // Display the solution
            std::cout << "Solution path (first 10 moves):\n";
            const auto& path = solver5.getPath();
            for (size_t i = 0; i < std::min(size_t(10), path.size()); ++i) {
                std::cout << "  Move " << (i + 1) << ": ("
                          << path[i].row << "," << path[i].col << ")\n";
            }
            if (path.size() > 10) {
                std::cout << "  ... (" << (path.size() - 10) << " more moves)\n";
            }
            std::cout << "\n";

            // Print the board with move numbers
            board5.print();
        } else {
            std::cout << "✗ No solution found\n";
            std::cout << "  Time: " << duration.count() << " ms\n";
            std::cout << "  Backtracks: " << solver5.getBacktrackCount() << "\n";
        }

        // Test on 6x6 board
        std::cout << "\n=== Testing on 6x6 Board ===\n\n";
        Board board6(6, 6);
        Solver solver6(board6);

        std::cout << "Attempting to solve 6x6 board starting at (0,0)...\n";

        start = std::chrono::high_resolution_clock::now();
        bool solved6 = solver6.solve(0, 0, TourType::OPEN);
        end = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        if (solved6) {
            std::cout << "✓ Solution found!\n";
            std::cout << "  Time: " << duration.count() << " ms\n";
            std::cout << "  Backtracks: " << solver6.getBacktrackCount() << "\n";
            std::cout << "  Moves: " << solver6.getPath().size() << "\n\n";

            board6.print();
        } else {
            std::cout << "✗ No solution found\n";
            std::cout << "  Time: " << duration.count() << " ms\n";
            std::cout << "  Backtracks: " << solver6.getBacktrackCount() << "\n";
        }

        // Note about 8x8
        std::cout << "\n=== Note ===\n";
        std::cout << "Without Warnsdorff's heuristic, solving an 8x8 board\n";
        std::cout << "takes too long (minutes to hours). This naive backtracking\n";
        std::cout << "demonstrates why we need intelligent move ordering!\n\n";
        std::cout << "Next commit will add Warnsdorff's heuristic for dramatic speedup.\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
