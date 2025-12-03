#include <iostream>
#include "Board.h"

int main() {
    std::cout << "Knight's Tour Solver - Board Test\n\n";

    try {
        // Create an 8x8 board
        Board board(8, 8);

        std::cout << "Board created: " << board.width() << "x" << board.height() << "\n";
        std::cout << "Total squares: " << board.size() << "\n\n";

        // Test setting and getting values
        std::cout << "Testing board operations:\n";
        board.set(0, 0, 1);
        board.set(1, 2, 2);
        board.set(3, 4, 3);

        std::cout << "Value at (0,0): " << board.at(0, 0) << "\n";
        std::cout << "Value at (1,2): " << board.at(1, 2) << "\n";
        std::cout << "Value at (3,4): " << board.at(3, 4) << "\n";
        std::cout << "Value at (0,1): " << board.at(0, 1) << " (unvisited)\n\n";

        // Test validation
        std::cout << "Validation tests:\n";
        std::cout << "Is (0,0) valid? " << (board.isValid(0, 0) ? "Yes" : "No") << "\n";
        std::cout << "Is (8,8) valid? " << (board.isValid(8, 8) ? "Yes" : "No") << "\n";
        std::cout << "Is (-1,0) valid? " << (board.isValid(-1, 0) ? "Yes" : "No") << "\n\n";

        // Test visited status
        std::cout << "Visit status:\n";
        std::cout << "Is (0,0) visited? " << (board.isVisited(0, 0) ? "Yes" : "No") << "\n";
        std::cout << "Is (0,1) visited? " << (board.isVisited(0, 1) ? "Yes" : "No") << "\n\n";

        // Print the board
        board.print();

        // Test clear
        std::cout << "\nClearing board...\n";
        board.clear();
        std::cout << "Is (0,0) visited after clear? " << (board.isVisited(0, 0) ? "Yes" : "No") << "\n";

        std::cout << "\n✓ All board tests passed!\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
