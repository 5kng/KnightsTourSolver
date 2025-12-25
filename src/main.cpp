#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <limits>
#include "Board.h"
#include "Solver.h"
#include "Exporter.h"

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printMenu() {
    std::cout << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║     KNIGHT'S TOUR SOLVER v2.0          ║\n";
    std::cout << "║     Christmas Day Edition              ║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";
    std::cout << "1. Solve custom board\n";
    std::cout << "2. Visualize solution (animated)\n";
    std::cout << "3. Export solution to file\n";
    std::cout << "4. Test all starting positions (8x8)\n";
    std::cout << "5. Quick solve (8×8 board)\n";
    std::cout << "0. Exit\n\n";
    std::cout << "Enter your choice: ";
}

void animateSolution(const Board& board, const std::vector<Move>& path) {
    std::cout << "\n🎬 Animating knight's journey...\n\n";
    
    for (size_t moveNum = 1; moveNum <= path.size(); ++moveNum) {
        // Clear screen
        std::cout << "\033[2J\033[H";
        
        std::cout << "Move " << moveNum << " / " << path.size() << "\n";
        std::cout << "Position: (" << path[moveNum-1].row << ", " << path[moveNum-1].col << ")\n\n";
        
        // Create temporary board showing progress
        std::vector<std::vector<int> > grid(board.height(), 
                                           std::vector<int>(board.width(), 0));
        for (size_t i = 0; i < moveNum; ++i) {
            grid[path[i].row][path[i].col] = i + 1;
        }
        
        // Print board
        for (size_t row = 0; row < board.height(); ++row) {
            for (size_t col = 0; col < board.width(); ++col) {
                if (grid[row][col] == 0) {
                    std::cout << "   .";
                } else {
                    std::cout << std::setw(4) << grid[row][col];
                }
            }
            std::cout << "\n";
        }
        
        // Pause between moves (faster for larger boards)
        int delayMs = path.size() > 36 ? 100 : 300;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
    
    std::cout << "\n✓ Tour complete! Press Enter to continue...";
    std::cin.get();
}

void quickSolve() {
    std::cout << "\n=== Quick Solve (8×8 Board) ===\n\n";
    Board board(8, 8);
    Solver solver(board);
    
    std::cout << "Solving from position (0, 0)...\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    bool solved = solver.solve(0, 0, TourType::OPEN);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    if (solved) {
        std::cout << "✓ Solution found!\n";
        std::cout << "  Time: " << duration.count() << " μs ("
                  << (duration.count() / 1000.0) << " ms)\n";
        std::cout << "  Backtracks: " << solver.getBacktrackCount() << "\n";
        std::cout << "  Moves: " << solver.getPath().size() << "\n\n";
        
        board.print();
    } else {
        std::cout << "✗ No solution found\n";
    }
}

void solveCustom() {
    int width, height, startRow, startCol;
    char tourTypeChoice;
    
    std::cout << "\n=== Custom Board Solver ===\n\n";
    
    std::cout << "Enter board width (5-20): ";
    std::cin >> width;
    std::cout << "Enter board height (5-20): ";
    std::cin >> height;
    
    if (width < 5 || width > 20 || height < 5 || height > 20) {
        std::cout << "Invalid board size. Must be between 5 and 20.\n";
        clearInput();
        return;
    }
    
    std::cout << "Enter starting row (0-" << (height-1) << "): ";
    std::cin >> startRow;
    std::cout << "Enter starting col (0-" << (width-1) << "): ";
    std::cin >> startCol;
    
    if (startRow < 0 || startRow >= height || startCol < 0 || startCol >= width) {
        std::cout << "Invalid starting position.\n";
        clearInput();
        return;
    }
    
    std::cout << "Tour type (O=Open, C=Closed): ";
    std::cin >> tourTypeChoice;
    TourType type = (tourTypeChoice == 'C' || tourTypeChoice == 'c') ? TourType::CLOSED : TourType::OPEN;
    
    clearInput();
    
    std::cout << "\nSolving " << width << "×" << height << " board from (" 
              << startRow << ", " << startCol << ")...\n";
    
    Board board(width, height);
    Solver solver(board);
    
    auto start = std::chrono::high_resolution_clock::now();
    bool solved = solver.solve(startRow, startCol, type);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    if (solved) {
        std::cout << "✓ Solution found!\n";
        std::cout << "  Time: " << duration.count() << " μs\n";
        std::cout << "  Backtracks: " << solver.getBacktrackCount() << "\n\n";
        
        board.print();
        
        // Ask if user wants to animate or export
        std::cout << "\nAnimate solution? (y/n): ";
        char animate;
        std::cin >> animate;
        if (animate == 'y' || animate == 'Y') {
            clearInput();
            animateSolution(board, solver.getPath());
        }
        
        std::cout << "\nExport solution? (y/n): ";
        char exportChoice;
        std::cin >> exportChoice;
        if (exportChoice == 'y' || exportChoice == 'Y') {
            std::cout << "Export format (1=JSON, 2=SVG, 3=Text): ";
            int format;
            std::cin >> format;
            
            std::string filename;
            if (format == 1) {
                filename = "knight_tour_solution.json";
                Exporter::exportToJSON(solver, board, filename);
            } else if (format == 2) {
                filename = "knight_tour_solution.svg";
                Exporter::exportToSVG(solver, board, filename);
            } else {
                filename = "knight_tour_solution.txt";
                Exporter::exportToText(solver, board, filename);
            }
            std::cout << "✓ Exported to " << filename << "\n";
        }
    } else {
        std::cout << "✗ No solution found\n";
        std::cout << "  Time: " << duration.count() << " μs\n";
        std::cout << "  Backtracks: " << solver.getBacktrackCount() << "\n";
    }
    
    clearInput();
}

void visualizeExisting() {
    std::cout << "\n=== Visualize Solution ===\n\n";
    std::cout << "Solving 8×8 board for visualization...\n";
    
    Board board(8, 8);
    Solver solver(board);
    
    if (solver.solve(0, 0, TourType::OPEN)) {
        animateSolution(board, solver.getPath());
    } else {
        std::cout << "Failed to find solution.\n";
    }
}

void exportSolution() {
    std::cout << "\n=== Export Solution ===\n\n";
    
    int width, height;
    std::cout << "Enter board width (5-20): ";
    std::cin >> width;
    std::cout << "Enter board height (5-20): ";
    std::cin >> height;
    
    if (width < 5 || width > 20 || height < 5 || height > 20) {
        std::cout << "Invalid board size.\n";
        clearInput();
        return;
    }
    
    Board board(width, height);
    Solver solver(board);
    
    std::cout << "Solving...\n";
    if (!solver.solve(0, 0, TourType::OPEN)) {
        std::cout << "Failed to find solution.\n";
        clearInput();
        return;
    }
    
    std::cout << "Solution found! Select export format:\n";
    std::cout << "1. JSON\n";
    std::cout << "2. SVG (Visual)\n";
    std::cout << "3. Plain Text\n";
    std::cout << "Choice: ";
    
    int format;
    std::cin >> format;
    
    std::string filename;
    bool success = false;
    
    if (format == 1) {
        filename = "knight_tour_solution.json";
        success = Exporter::exportToJSON(solver, board, filename);
    } else if (format == 2) {
        filename = "knight_tour_solution.svg";
        success = Exporter::exportToSVG(solver, board, filename);
    } else {
        filename = "knight_tour_solution.txt";
        success = Exporter::exportToText(solver, board, filename);
    }
    
    if (success) {
        std::cout << "✓ Successfully exported to " << filename << "\n";
    } else {
        std::cout << "✗ Export failed\n";
    }
    
    clearInput();
}

void testAllPositions() {
    std::cout << "\n=== Testing All Starting Positions (8×8) ===\n\n";
    std::cout << "Testing all 64 possible starting positions...\n";
    
    int successCount = 0;
    long long totalTime = 0;
    size_t totalBacktracks = 0;
    long long minTime = 1000000;
    long long maxTime = 0;
    Move fastestStart = {0, 0};
    Move slowestStart = {0, 0};
    
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Board testBoard(8, 8);
            Solver testSolver(testBoard);
            
            auto startTime = std::chrono::high_resolution_clock::now();
            bool solved = testSolver.solve(row, col, TourType::OPEN);
            auto endTime = std::chrono::high_resolution_clock::now();
            
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
            
            if (solved) {
                ++successCount;
                totalTime += elapsed;
                totalBacktracks += testSolver.getBacktrackCount();
                
                if (elapsed < minTime) {
                    minTime = elapsed;
                    fastestStart = {row, col};
                }
                if (elapsed > maxTime) {
                    maxTime = elapsed;
                    slowestStart = {row, col};
                }
            }
        }
    }
    
    std::cout << "\n✓ Results:\n";
    std::cout << "  Success rate: " << successCount << "/64 positions ("
              << (100.0 * successCount / 64) << "%)\n";
    std::cout << "  Avg time: " << (totalTime / successCount) << " μs\n";
    std::cout << "  Min time: " << minTime << " μs at position ("
              << fastestStart.row << "," << fastestStart.col << ")\n";
    std::cout << "  Max time: " << maxTime << " μs at position ("
              << slowestStart.row << "," << slowestStart.col << ")\n";
    std::cout << "  Avg backtracks: " << (totalBacktracks / successCount) << "\n";
}

int main() {
    std::cout << "\033[2J\033[H"; // Clear screen

    try {
        int choice = -1;
        
        while (choice != 0) {
            printMenu();
            std::cin >> choice;
            
            if (std::cin.fail()) {
                clearInput();
                std::cout << "Invalid input. Please enter a number.\n";
                continue;
            }
            
            clearInput();
            
            switch (choice) {
                case 1:
                    solveCustom();
                    break;
                case 2:
                    visualizeExisting();
                    break;
                case 3:
                    exportSolution();
                    break;
                case 4:
                    testAllPositions();
                    break;
                case 5:
                    quickSolve();
                    break;
                case 0:
                    std::cout << "\nThank you for using Knight's Tour Solver!\n";
                    std::cout << "Merry Christmas! 🎄\n\n";
                    break;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
            
            if (choice != 0) {
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
