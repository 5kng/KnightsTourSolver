#pragma once

#include "Board.h"
#include "Solver.h"
#include <string>
#include <fstream>

/**
 * @brief Export knight's tour solutions to various file formats
 */
class Exporter {
public:
    /**
     * @brief Export solution to JSON format
     * @param solver Solver containing the solution
     * @param board Board with the solution
     * @param filename Output filename
     * @return true if export successful
     */
    static bool exportToJSON(const Solver& solver, const Board& board, const std::string& filename);

    /**
     * @brief Export solution to SVG format (visual representation)
     * @param solver Solver containing the solution
     * @param board Board with the solution
     * @param filename Output filename
     * @return true if export successful
     */
    static bool exportToSVG(const Solver& solver, const Board& board, const std::string& filename);

    /**
     * @brief Export solution to plain text format
     * @param solver Solver containing the solution
     * @param board Board with the solution
     * @param filename Output filename
     * @return true if export successful
     */
    static bool exportToText(const Solver& solver, const Board& board, const std::string& filename);

private:
    /**
     * @brief Escape special characters for JSON strings
     * @param str String to escape
     * @return Escaped string
     */
    static std::string escapeJSON(const std::string& str);
};
