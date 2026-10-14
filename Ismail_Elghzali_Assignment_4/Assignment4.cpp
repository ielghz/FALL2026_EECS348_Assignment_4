/*
EECS 348 Assignment 4 - Sudoku Solver

Description:Reads five Sudoku puzzle files and solves them using recursive depth-first search with backtracking.
 
Input: Sudoku puzzle text files containing numbers and _ for blanks.


Output: Prints the filename, original puzzle grid, and all valid solutions, or prints "No solution found" if unsolvable.


Collaborators: None

Other Sources: Perplexity Generative AI (as required by assignment), Gemini AI

Author: Ismail

Creation Date: 13 October, 2026
Revision Date: 13 October, 2026

Revisions: Added prolouge and inline comments.
*/

#include <array> // Includes fixed-size container library
#include <fstream> // Includes file stream library for reading files
#include <iostream> // Includes input-output library for printing
#include <string> // Includes string library for text handling

class SudokuPuzzle { // Declares the SudokuPuzzle class for grid management
private: // Begins private member section
    enum { SIZE = 9, BOX_SIZE = 3, CELL_COUNT = SIZE * SIZE }; // Sets board dimension constants
    using Grid = std::array<std::array<int, SIZE>, SIZE>; // Creates type alias for 9x9 grid

    Grid original_{}; // Initializes grid to store original puzzle values
    Grid board_{}; // Initializes working board grid for search
    bool foundSolution_ = false; // Initializes solution tracking flag

    void printGrid(const Grid& grid, std::ostream& out) const {
        for (int row = 0; row < SIZE; ++row) { // Loops through each row of the grid
            for (int col = 0; col < SIZE; ++col) { // Loops through each column of the grid
                if (col != 0) { // Checks if not the first column
                    out << ' '; // Prints space between grid cells
                }
                if (grid[row][col] == 0) { // Checks if the cell is empty
                    out << '_'; // Prints underscore for empty cell
                } else {
                    out << grid[row][col]; // Prints the numeric digit
                }
            }
            out << '\n'; // Prints a newline at the end of the row
        }
    }

    bool canPlace(int row, int col, int value) const {
        for (int index = 0; index < SIZE; ++index) { // Loops through row and column indices
            if (index != col && board_[row][index] == value) { // Checks row for duplicate value
                return false; // Returns false if value already exists in row
            }
            if (index != row && board_[index][col] == value) { // Checks column for duplicate value
                return false; // Returns false if value already exists in column
            }
        }

        const int firstRow = (row / BOX_SIZE) * BOX_SIZE; // Calculates starting row of 3x3 block
        const int firstCol = (col / BOX_SIZE) * BOX_SIZE; // Calculates starting column of 3x3 block
        for (int r = firstRow; r < firstRow + BOX_SIZE; ++r) { // Loops through 3x3 block rows
            for (int c = firstCol; c < firstCol + BOX_SIZE; ++c) { // Loops through 3x3 block columns
                if ((r != row || c != col) && board_[r][c] == value) { // Checks 3x3 block for duplicate value
                    return false; // Returns false if value exists in block
                }
            }
        }
        return true; // Returns true if placement is valid
    }

    void search(std::ostream& out) {
        int chosenRow = -1; // Initializes chosen row variable
        int chosenCol = -1; // Initializes chosen column variable
        int fewestCandidates = SIZE + 1; // Initializes minimum candidate tracker
        std::array<int, SIZE> chosenCandidates{}; // Initializes array for best cell candidates

        for (int row = 0; row < SIZE; ++row) { // Loops through all rows
            for (int col = 0; col < SIZE; ++col) { // Loops through all columns
                if (board_[row][col] != 0) { // Checks if cell is already filled
                    continue; // Skips non-empty cells
                }
                std::array<int, SIZE> candidates{}; // Creates candidate container
                int count = 0; // Initializes candidate count
                for (int value = 1; value <= SIZE; ++value) { // Loops through numbers 1 to 9
                    if (canPlace(row, col, value)) { // Checks if number can be placed
                        candidates[count++] = value; // Stores valid candidate digit
                    }
                }
                if (count == 0) { // Checks if cell has zero valid candidates
                    return; // Returns early to trigger backtracking
                }
                if (count < fewestCandidates) { // Checks if current cell has fewer candidates
                    chosenRow = row; // Updates selected row
                    chosenCol = col; // Updates selected column
                    fewestCandidates = count; // Updates minimum candidate count
                    chosenCandidates = candidates; // Updates saved candidate list
                }
            }
        }

        if (chosenRow == -1) { // Checks if all cells are filled
            foundSolution_ = true; // Sets solution found flag to true
            out << "Solution:\n"; // Prints solution label
            printGrid(board_, out); // Prints the completed board
            out << '\n'; // Prints newline
            return; // Returns from current recursive branch
        }

        for (int index = 0; index < fewestCandidates; ++index) { // Loops through valid candidates
            board_[chosenRow][chosenCol] = chosenCandidates[index]; // Places candidate on board
            search(out); // Recurses to solve the next cell
            board_[chosenRow][chosenCol] = 0; // Resets cell back to empty during backtrack
        }
    }

public:
    bool load(const std::string& filename, std::string& error) {
        error.clear(); // Clears any previous error message
        original_ = Grid{}; // Resets original puzzle grid
        board_ = Grid{}; // Resets working puzzle grid
        foundSolution_ = false; // Resets solution status flag
        std::ifstream input(filename); // Opens file stream for reading
        if (!input) { // Checks if file stream failed to open
            error = "Cannot open puzzle file."; // Sets file open error message
            return false; // Returns false if file cannot be opened
        }

        Grid parsed{}; // Creates temporary grid container
        int count = 0; // Initializes cell counter
        char token = '\0'; // Initializes token character variable
        while (input >> token) { // Reads tokens from file skipping whitespace
            if (count >= CELL_COUNT) { // Checks if file has extra data beyond 81 cells
                error = "Expected exactly 81 cells; found extra data."; // Sets size error message
                return false; // Returns false for malformed input
            }
            if (token != '_' && (token < '1' || token > '9')) { // Checks for invalid character tokens
                error = "Invalid character found."; // Sets invalid character error message
                return false; // Returns false for invalid input
            }
            parsed[count / SIZE][count % SIZE] = (token == '_') ? 0 : token - '0'; // Maps token to grid cell
            ++count; // Increments total cell count
        }
        if (input.bad() || (input.fail() && !input.eof())) { // Checks for file stream I/O errors
            error = "An I/O error occurred."; // Sets I/O error message
            return false; // Returns false on stream failure
        }
        if (count != CELL_COUNT) { // Checks if file has fewer than 81 cells
            error = "Incomplete data found."; // Sets incomplete data error message
            return false; // Returns false for incomplete input
        }
        original_ = parsed; // Stores parsed grid as original givens
        board_ = parsed; // Initializes working board with givens
        return true; // Returns true upon successful loading
    }

    bool validate(std::string& error) const {
        error.clear(); // Clears error string
        for (int row = 0; row < SIZE; ++row) { // Loops through each board row
            for (int col = 0; col < SIZE; ++col) { // Loops through each board column
                const int value = board_[row][col]; // Retrieves cell value
                if (value != 0 && !canPlace(row, col, value)) { // Checks if initial given violates rules
                    error = "Conflicting given found."; // Sets conflict error message
                    return false; // Returns false if initial conflict exists
                }
            }
        }
        return true; // Returns true if all initial givens are valid
    }

    bool printOriginal(std::ostream& out) const {
        printGrid(original_, out); // Calls printGrid helper for original board
        return true; // Returns true
    }

    void solveAll(std::ostream& out) {
        board_ = original_; // Resets working board to original givens
        foundSolution_ = false; // Resets solution flag
        std::string error; // Creates local error string
        if (!validate(error)) { // Validates starting board state
            out << "Error: " << error << '\n'; // Prints validation error
            out << "No solution found\n"; // Prints failure message
            return; // Returns early if validation fails
        }
        search(out); // Begins recursive backtracking search
        if (!foundSolution_) { // Checks if no solution was found during search
            out << "No solution found\n"; // Prints failure message
        }
    }
};

class SudokuApplication {
public:
    int run() const {
        const std::array<std::string, 5> filenames = {{ // Defines array of required puzzle filenames
            "puzzle1.txt", "puzzle2.txt", "puzzle3.txt",
            "puzzle4.txt", "puzzle5.txt"
        }};
        bool inputError = false; // Initializes application error tracker flag
        for (const std::string& filename : filenames) { // Loops through each puzzle filename
            std::cout << "Puzzle file: " << filename << '\n'; // Prints current filename header
            SudokuPuzzle puzzle; // Creates SudokuPuzzle object instance
            std::string error; // Initializes error string
            if (!puzzle.load(filename, error)) { // Attempts to load puzzle file
                std::cout << "Error: " << error << "\n\n"; // Prints load error message
                inputError = true; // Sets error flag to true
                continue; // Skips to next file loop iteration
            }
            std::cout << "Original puzzle:\n"; // Prints original puzzle header label
            puzzle.printOriginal(std::cout); // Prints original puzzle layout
            std::cout << '\n'; // Prints newline
            if (!puzzle.validate(error)) { // Validates starting puzzle givens
                std::cout << "Error: " << error << '\n'; // Prints validation error message
                std::cout << "No solution found\n\n"; // Prints required no solution message
                inputError = true; // Sets error flag to true
                continue; // Skips to next file loop iteration
            }
            puzzle.solveAll(std::cout); // Solves puzzle and prints all valid solutions
            std::cout << '\n'; // Prints newline
        }
        return inputError ? 1 : 0; // Returns exit status code based on errors
    }
};

int main() {
    SudokuApplication application; // Creates SudokuApplication object instance
    return application.run(); // Runs application and returns exit status code
}