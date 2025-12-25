#include "Exporter.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

bool Exporter::exportToJSON(const Solver& solver, const Board& board, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return false;
    }

    const auto& path = solver.getPath();
    auto stats = solver.getPathStatistics();

    file << "{\n";
    file << "  \"board\": {\n";
    file << "    \"width\": " << board.width() << ",\n";
    file << "    \"height\": " << board.height() << "\n";
    file << "  },\n";
    file << "  \"solution\": {\n";
    file << "    \"moves\": " << path.size() << ",\n";
    file << "    \"backtracks\": " << solver.getBacktrackCount() << ",\n";
    file << "    \"path\": [\n";
    
    for (size_t i = 0; i < path.size(); ++i) {
        file << "      {\"row\": " << path[i].row << ", \"col\": " << path[i].col << "}";
        if (i < path.size() - 1) file << ",";
        file << "\n";
    }
    
    file << "    ],\n";
    file << "    \"statistics\": {\n";
    file << "      \"cornerVisits\": " << stats.cornerVisits << ",\n";
    file << "      \"edgeVisits\": " << stats.edgeVisits << ",\n";
    file << "      \"centerVisits\": " << stats.centerVisits << ",\n";
    file << "      \"avgDistanceFromCenter\": " << stats.averageDistanceFromCenter << "\n";
    file << "    }\n";
    file << "  }\n";
    file << "}\n";

    file.close();
    return true;
}

bool Exporter::exportToSVG(const Solver& solver, const Board& board, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return false;
    }

    const auto& path = solver.getPath();
    const int cellSize = 60;
    const int padding = 40;
    const int width = board.width() * cellSize + 2 * padding;
    const int height = board.height() * cellSize + 2 * padding;

    // SVG header
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    file << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << width 
         << "\" height=\"" << height << "\">\n";
    
    // Title
    file << "  <text x=\"" << width/2 << "\" y=\"25\" text-anchor=\"middle\" "
         << "font-family=\"Arial\" font-size=\"18\" font-weight=\"bold\">"
         << "Knight's Tour Solution (" << board.width() << "×" << board.height() << ")"
         << "</text>\n\n";

    // Draw chessboard
    file << "  <!-- Chessboard -->\n";
    for (int row = 0; row < static_cast<int>(board.height()); ++row) {
        for (int col = 0; col < static_cast<int>(board.width()); ++col) {
            int x = padding + col * cellSize;
            int y = padding + row * cellSize;
            bool isLight = (row + col) % 2 == 0;
            file << "  <rect x=\"" << x << "\" y=\"" << y 
                 << "\" width=\"" << cellSize << "\" height=\"" << cellSize 
                 << "\" fill=\"" << (isLight ? "#f0d9b5" : "#b58863") << "\"/>\n";
        }
    }

    // Draw path lines
    file << "\n  <!-- Path lines -->\n";
    file << "  <g stroke=\"#2196F3\" stroke-width=\"3\" stroke-opacity=\"0.6\" "
         << "fill=\"none\" stroke-linecap=\"round\">\n";
    
    for (size_t i = 0; i < path.size() - 1; ++i) {
        int x1 = padding + path[i].col * cellSize + cellSize / 2;
        int y1 = padding + path[i].row * cellSize + cellSize / 2;
        int x2 = padding + path[i + 1].col * cellSize + cellSize / 2;
        int y2 = padding + path[i + 1].row * cellSize + cellSize / 2;
        
        file << "    <line x1=\"" << x1 << "\" y1=\"" << y1 
             << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\"/>\n";
    }
    file << "  </g>\n";

    // Draw move numbers
    file << "\n  <!-- Move numbers -->\n";
    for (size_t i = 0; i < path.size(); ++i) {
        int x = padding + path[i].col * cellSize + cellSize / 2;
        int y = padding + path[i].row * cellSize + cellSize / 2;
        
        // Circle background
        std::string fillColor = (i == 0) ? "#4CAF50" : (i == path.size() - 1) ? "#F44336" : "#FFF";
        file << "  <circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"18\" "
             << "fill=\"" << fillColor << "\" stroke=\"#333\" stroke-width=\"2\"/>\n";
        
        // Move number
        file << "  <text x=\"" << x << "\" y=\"" << (y + 5) 
             << "\" text-anchor=\"middle\" font-family=\"Arial\" "
             << "font-size=\"14\" font-weight=\"bold\" fill=\"#333\">"
             << (i + 1) << "</text>\n";
    }

    // Legend
    file << "\n  <!-- Legend -->\n";
    int legendY = height - 15;
    file << "  <circle cx=\"20\" cy=\"" << legendY << "\" r=\"8\" fill=\"#4CAF50\"/>\n";
    file << "  <text x=\"35\" y=\"" << (legendY + 4) << "\" font-family=\"Arial\" font-size=\"12\">Start</text>\n";
    file << "  <circle cx=\"90\" cy=\"" << legendY << "\" r=\"8\" fill=\"#F44336\"/>\n";
    file << "  <text x=\"105\" y=\"" << (legendY + 4) << "\" font-family=\"Arial\" font-size=\"12\">End</text>\n";

    file << "</svg>\n";
    file.close();
    return true;
}

bool Exporter::exportToText(const Solver& solver, const Board& board, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return false;
    }

    const auto& path = solver.getPath();
    auto stats = solver.getPathStatistics();

    file << "KNIGHT'S TOUR SOLUTION\n";
    file << "======================\n\n";
    file << "Board Size: " << board.width() << " × " << board.height() << "\n";
    file << "Total Moves: " << path.size() << "\n";
    file << "Backtracks: " << solver.getBacktrackCount() << "\n\n";

    file << "STATISTICS\n";
    file << "----------\n";
    file << "Corner Visits: " << stats.cornerVisits << "\n";
    file << "Edge Visits: " << stats.edgeVisits << "\n";
    file << "Center Visits: " << stats.centerVisits << "\n";
    file << "Avg Distance from Center: " << std::fixed << std::setprecision(2) 
         << stats.averageDistanceFromCenter << "\n\n";

    file << "MOVE SEQUENCE\n";
    file << "-------------\n";
    for (size_t i = 0; i < path.size(); ++i) {
        file << "Move " << std::setw(3) << (i + 1) << ": ("
             << std::setw(2) << path[i].row << ", " 
             << std::setw(2) << path[i].col << ")\n";
    }

    file << "\nBOARD VISUALIZATION\n";
    file << "-------------------\n";
    
    // Print board with move numbers
    std::vector<std::vector<int>> boardGrid(board.height(), 
                                             std::vector<int>(board.width(), 0));
    for (size_t i = 0; i < path.size(); ++i) {
        boardGrid[path[i].row][path[i].col] = i + 1;
    }

    for (size_t row = 0; row < board.height(); ++row) {
        for (size_t col = 0; col < board.width(); ++col) {
            file << std::setw(4) << boardGrid[row][col];
        }
        file << "\n";
    }

    file.close();
    return true;
}

std::string Exporter::escapeJSON(const std::string& str) {
    std::ostringstream oss;
    for (char c : str) {
        switch (c) {
            case '"':  oss << "\\\""; break;
            case '\\': oss << "\\\\"; break;
            case '\b': oss << "\\b"; break;
            case '\f': oss << "\\f"; break;
            case '\n': oss << "\\n"; break;
            case '\r': oss << "\\r"; break;
            case '\t': oss << "\\t"; break;
            default:   oss << c; break;
        }
    }
    return oss.str();
}
