/**
 * @file Assignment4.cpp
 * @brief Recursive object-oriented Sudoku Solver using Depth-First Search (DFS) and backtracking.
 * @author [Your Full Name]
 * @date October 13, 2026
 * 
 * Course: EECS 348 - Software Engineering I
 * Assignment: Assignment No. 4
 * Inputs: Sudoku puzzle text files (puzzle1.txt to puzzle5.txt) containing 9x9 grids with '_' for blanks.
 * Outputs: Prints the filename, original puzzle layout, and all valid solved configurations (or "No solution found").
 * Collaborators: None
 * Sources: Generated with assistance from AI (Gemini) as part of the GenAI comparison requirement.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Class encapsulating the Sudoku board state and resolution operations
class SudokuSolver {
private:
    char board[9][9];     // 2D array representing the 9x9 grid layout
    int solutionCounter;  // Track number of valid completions discovered

    // Validates if placing a specific digit at board[row][col] respects Sudoku rules
    bool isValidPlacement(int row, int col, char num) {
        // Inspect row and column constraints simultaneously
        for (int i = 0; i < 9; ++i) {
            if (board[row][i] == num) return false; // Number already exists in this row
            if (board[i][col] == num) return false; // Number already exists in this column
        }

        // Establish boundaries for the localized 3x3 box region
        int subgridRowStart = (row / 3) * 3;
        int subgridColStart = (col / 3) * 3;

        // Inspect 3x3 local block constraints
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[subgridRowStart + i][subgridColStart + j] == num) {
                    return false; // Number already exists within this 3x3 sub-grid
                }
            }
        }
        return true; // No rule violations detected
    }

    // Core depth-first search execution sequence utilizing backtracking
    void findSolutions(int row, int col) {
        // If we transcend row index 8, a complete configuration has been populated
        if (row == 9) {
            solutionCounter++;
            std::cout << "\n--- Solution " << solutionCounter << " ---\n";
            displayGrid();
            return;
        }

        // Advance to the subsequent row when the right edge of the board is breached
        if (col == 9) {
            findSolutions(row + 1, 0);
            return;
        }

        // Advance immediately if the current index is already occupied by a fixed hint
        if (board[row][col] != '_') {
            findSolutions(row, col + 1);
            return;
        }

        // Iteratively attempt placing numbers '1' through '9' into the blank matrix location
        for (char num = '1'; num <= '9'; ++num) {
            if (isValidPlacement(row, col, num)) {
                board[row][col] = num;       // Assign the candidate value tentatively
                findSolutions(row, col + 1); // Explore subsequent states recursively
                board[row][col] = '_';       // Revert assignment (backtrack) if branch stalls
            }
        }
    }

public:
    // Constructor handles baseline parameter normalization
    SudokuSolver() {
        solutionCounter = 0;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                board[i][j] = '_';
            }
        }
    }

    // Securely streams raw grid data from the text file into our member variable grid
    bool parsePuzzleFile(const std::string& targetFile) {
        std::ifstream fileStream(targetFile);
        if (!fileStream.is_open()) {
            std::cerr << "Error: Target puzzle file '" << targetFile << "' could not be reached.\n";
            return false;
        }
        
        solutionCounter = 0; // Reset metrics context prior to parsing sequence
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (!(fileStream >> board[i][j])) {
                    return false; // Premature end of file or reading failure
                }
            }
        }
        fileStream.close();
        return true;
    }

    // Prints current snapshot values held in memory inside clean standard format outputs
    void displayGrid() const {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                std::cout << board[i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    // Front-facing execution method managing workflow wrapper logging
    void executeSolvingSequence(const std::string& targetFile) {
        std::cout << "========================================\n";
        std::cout << "Puzzle File: " << targetFile << "\n";
        std::cout << "Original Puzzle Layout:\n";
        displayGrid();
        
        findSolutions(0, 0); // Initialize targeted backtracking root evaluation
        
        if (solutionCounter == 0) {
            std::cout << "\nNo solution found\n";
        }
        std::cout << "========================================\n\n";
    }
};

int main() {
    // Process target input matrix list sequentially 
    std::string continuousPuzzleQueue[] = {"puzzle1.txt", "puzzle2.txt", "puzzle3.txt", "puzzle4.txt", "puzzle5.txt"};
    
    for (const std::string& activeFile : continuousPuzzleQueue) {
        SudokuSolver operatingEngine;
        if (operatingEngine.parsePuzzleFile(activeFile)) {
            operatingEngine.executeSolvingSequence(activeFile);
        }
    }
    return 0;
}
