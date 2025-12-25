# Knight's Tour Solver

A high-performance C++20 implementation of the Knight's Tour problem using Warnsdorff's heuristic with backtracking.

## Overview

The Knight's Tour is a classic chess problem where a knight must visit every square on a chessboard exactly once. This solver implements an optimized algorithm achieving sub-millisecond performance on standard 8×8 boards with a 100% success rate.

## Features

### Core Solver
- **High Performance**: Sub-millisecond solve times (~55μs on 8×8 boards)
- **Scalable**: Supports board sizes from 5×5 up to 100×100
- **Multiple Modes**: Open and closed tour solutions
- **100% Success Rate**: Solves from any starting position on standard boards
- **Modern C++20**: Leverages latest C++ features for clean, efficient code

### Interactive Features (Day 5 - Christmas Edition!)
- **Interactive CLI Menu**: User-friendly interface for all features
- **Solution Export**: Export tours to JSON, SVG (visual), or plain text
- **Animated Visualization**: Watch the knight's journey step-by-step
- **Comprehensive Testing**: Test all 64 starting positions on an 8×8 board
- **Custom Board Solver**: Choose your own board size and starting position

## Algorithm

The solver uses Warnsdorff's heuristic, a greedy algorithm that chooses the next move to the square with the fewest onward moves. This is combined with backtracking to guarantee a solution on solvable boards.

## Performance

- **8×8 Board**: ~55μs average solve time
- **Success Rate**: 100% from all 64 starting positions
- **Zero Backtracking**: Warnsdorff's heuristic with look-ahead pruning
- **Scalability**: Efficient solving up to 100×100 boards

### Benchmark Results (8×8 Board)
```
✓ Success rate: 64/64 positions (100%)
  Avg time: ~55 μs
  Min time: ~30 μs
  Max time: ~200 μs
  Avg backtracks: 0
```

## Project Status

🚧 **Under Development** - Following the 7-day build plan

## Requirements

- C++20 compatible compiler (GCC 10+, Clang 10+, or MSVC 2019+)
- CMake 3.15 or higher

## Building

### Linux / macOS

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
./knights_tour
```

### Windows (Visual Studio)

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
.\Release\knights_tour.exe
```

### Build Types

- **Release** (default): Optimized for performance (-O3, LTO enabled)
- **Debug**: Includes debugging symbols and address sanitizer

To build in debug mode:
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

## Usage

### Interactive Mode

Run the program and choose from the interactive menu:

```bash
./knights_tour
```

**Menu Options:**
1. **Solve custom board** - Choose board size, starting position, and tour type
2. **Visualize solution (animated)** - Watch an animated ASCII tour on 8×8 board
3. **Export solution to file** - Save tours in JSON, SVG, or text format
4. **Test all starting positions** - Benchmark all 64 positions on 8×8 board
5. **Quick solve** - Instantly solve an 8×8 board from (0,0)

### Export Formats

**JSON** - Structured data with path, statistics, and board info
```json
{
  "board": {"width": 8, "height": 8},
  "solution": {
    "moves": 64,
    "backtracks": 0,
    "path": [{"row": 0, "col": 0}, ...],
    "statistics": {...}
  }
}
```

**SVG** - Beautiful visual representation with chessboard and path
- Green circle: Starting position
- Red circle: Ending position
- Blue lines: Knight's path
- Open in any web browser

**Text** - Human-readable format with move sequence and board visualization

### Example Session

```
Enter your choice: 1

=== Custom Board Solver ===

Enter board width (5-20): 8
Enter board height (5-20): 8
Enter starting row (0-7): 0
Enter starting col (0-7): 0
Tour type (O=Open, C=Closed): O

Solving 8×8 board from (0, 0)...
✓ Solution found!
  Time: 55 μs
  Backtracks: 0
```

## License

MIT License - see LICENSE file for details.
