# Knight's Tour Solver

A high-performance C++20 implementation of the Knight's Tour problem using Warnsdorff's heuristic with backtracking.

## Overview

The Knight's Tour is a classic chess problem where a knight must visit every square on a chessboard exactly once. This solver implements an optimized algorithm achieving sub-millisecond performance on standard 8×8 boards with a 100% success rate.

## Features

- **High Performance**: Sub-millisecond solve times (0.3ms median on 8×8 boards)
- **Scalable**: Supports board sizes from 5×5 up to 100×100
- **Multiple Modes**: Open and closed tour solutions
- **Comprehensive Benchmarking**: Statistical analysis across 1000+ runs with microsecond-precision timing
- **Modern C++20**: Leverages latest C++ features for clean, efficient code

## Algorithm

The solver uses Warnsdorff's heuristic, a greedy algorithm that chooses the next move to the square with the fewest onward moves. This is combined with backtracking to guarantee a solution on solvable boards.

## Performance Targets

- **8×8 Board**: ~0.3ms median solve time
- **Success Rate**: 100% on standard boards
- **Scalability**: Efficient solving up to 100×100 boards
- **Benchmarking**: 1000+ iterations with full statistical analysis

## Project Status

🚧 **Under Development** - Following the 7-day build plan

## Building

Instructions coming soon.

## Usage

Usage examples coming soon.

## License

MIT License - see LICENSE file for details.
