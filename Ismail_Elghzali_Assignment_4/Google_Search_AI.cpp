/**
 * @file Assignment4.cpp
 * @brief Fully encapsulated Object-Oriented C++ Sudoku Solver using Backtracking DFS.
 * @author [Your Full Name]
 * @date October 13, 2026
 * 
 * Course: EECS 348 - Software Engineering I
 * Assignment: Assignment No. 4
 * Inputs: Nine-by-nine Sudoku text grids (puzzle1.txt to puzzle5.txt) with '_' for empty cells.
 * Outputs: Prints the filename, original grid state, and all unique valid solved boards.
 * Collaborators: [Placeholders for Collaborators]
 * Sources: Developed with architectural assistance from AI to fulfill the GenAI evaluation requirement.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>

class SudokuBoard {
private:
    char grid[9][9];
    bool isOriginal[9][9];
    int solutionCount;

    // Checks if a candidate digit can be safely placed at grid[row][col]
    bool isValid(int row, int col, char num) const {
        for (int i = 0; i < 9; ++i) {
            // Check row constraint
            if (grid[row][i] == num) return false;
            // Check column constraint
            if (grid[i][col] == num) return false;
        }

        // Check 3-by-3 block subgrid constraint
        int boxRowStart = (row / 3) * 3;
        int boxColStart = (col / 3) * 3;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (grid[boxRowStart + i][boxColStart + j] == num) return false;
            }
        }
        return true;
    }

    // Recursive depth-first search solver function
    void solveCell(int row, int col) {
        // If we step past row 8, we have successfully filled out the entire board
        if (row == 9) {
            solutionCount++;
            std::cout << "\nSolution " << solutionCount << ":\n";
            printGrid();
            return;
        }

        // Advance to the next row if we exceed the right-most column
        if (col == 9) {
            solveCell(row + 1, 0);
            return;
        }

        // Skip cells that contained given constraints from the file
        if (grid[row][col] != '_') {
            solveCell(row, col + 1);
            return;
        }

        // Brute-force try digits '1' through '9'
        for (char num = '1'; num <= '9'; ++num) {
            if (isValid(row, col, num)) {
                grid[row][col] = num;       // Assign tentatively
                solveCell(row, col + 1);    // Recurse to next cell
                grid[row][col] = '_';       // Reset state (backtrack)
            }
        }
    }

public:
    SudokuBoard() : solutionCount(0) {
        clearBoard();
    }

    void clearBoard() {
        solutionCount = 0;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                grid[i][j] = '_';
                isOriginal[i][j] = false;
            }
        }
    }

    // Handles file ingestion, parsing logic, and integrity verification
    bool loadFromFile(const std::string& filename) {
        clearBoard();
        std::ifstream inFile(filename);
        if (!inFile.is_open()) {
            std::cerr << "Error: Could not open file " << filename << "\n";
            return false;
        }

        char ch;
        int cellCount = 0;
        while (inFile >> ch) {
            if (ch == '_' || (ch >= '1' && ch <= '9')) {
                if (cellCount >= 81) {
                    std::cerr << "Malformed Input Error: File " << filename << " contains more than 81 board cells.\n";
                    return false;
                }
                int r = cellCount / 9;
                int c = cellCount % 9;
                grid[r][c] = ch;
                if (ch != '_') {
                    isOriginal[r][c] = true;
                }
                cellCount++;
            } else {
                std::cerr << "Malformed Input Error: Invalid character '" << ch << "' encountered in " << filename << ".\n";
                return false;
            }
        }

        if (cellCount < 81) {
            std::cerr << "Malformed Input Error: File " << filename << " has incomplete data (found " << cellCount << "/81 cells).\n";
            return false;
        }

        // Validate that initial pre-filled cells do not violate basic Sudoku rules
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (isOriginal[r][c]) {
                    char temp = grid[r][c];
                    grid[r][c] = '_'; // Temporarily lift out cell value to run standard check
                    if (!isValid(r, c, temp)) {
                        std::cerr << "Conflicting Given Digits Error: Inherent duplicate values found in " << filename << ".\n";
                        return false;
                    }
                    grid[r][c] = temp; // Restore verified hint
                }
            }
        }
        return true;
    }

    void printGrid() const {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                std::cout << grid[i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    // Encapsulated control sequence managing execution state boundaries
    void processPuzzle(const std::string& filename) {
        std::cout << "========================================\n";
        std::cout << "Processing File: " << filename << "\n";
        
        if (!loadFromFile(filename)) {
            std::cout << "Skipping file due to execution errors.\n";
            std::cout << "========================================\n\n";
            return;
        }

        std::cout << "Original Puzzle Layout:\n";
        printGrid();

        solveCell(0, 0);

        if (solutionCount == 0) {
            std::cout << "\nNo solution found\n";
        }
        std::cout << "========================================\n\n";
    }
};

int main() {
    std::string puzzleFiles[] = {"puzzle1.txt", "puzzle2.txt", "puzzle3.txt", "puzzle4.txt", "puzzle5.txt"};
    SudokuBoard solver;

    for (const std::string& file : puzzleFiles) {
        solver.processPuzzle(file);
    }

    return 0;
}
