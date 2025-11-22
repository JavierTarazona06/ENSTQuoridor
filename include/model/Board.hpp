#pragma once

#include <vector>
#include <array>
#include <stdexcept>
#include <iostream>

namespace Quoridor {
    
    // Define board dimensions
    constexpr int BOARD_SIZE = 9;
    constexpr int NUM_PLAYERS = 2;
    
    // Pawn position struct (x: column [0-8], y: row [0-8])
    struct Position {
        int x;
        int y;
        
        // C++17 explicit equality operator
        bool operator==(const Position& other) const {
            return x == other.x && y == other.y;
        }
    };

    // Wall orientation
    enum class Orientation {
        Horizontal, 
        Vertical    
    };

    // Wall struct: position is defined by the top-left square coordinate of the wall
    struct Wall {
        Position pos;
        Orientation orientation;

        // C++17 explicit equality operator
        bool operator==(const Wall& other) const {
            // Note: Since Position has its own == operator, we can compare it directly.
            return pos == other.pos && orientation == other.orientation;
        }
    };

    // Cell struct to represent a single cell on the board
    struct Cell {
        int x;
        int y;
        // Additional properties can be added here if needed
    };

    class Board {
    public:
        /**
         * @brief Constructor: Initializes the board state
         * Player 0 starts at (4, 0) (top row center)
         * Player 1 starts at (4, 8) (bottom row center)
         */
        Board();

        /**
         * @brief Moves a pawn to a new position.
         * @param playerIndex Player index (0 or 1)
         * @param newPos New position
         */
        void movePawn(int playerIndex, const Position& newPos);

        /**
         * @brief Sets the position of a player's pawn.
         * @param player Player index (0 or 1)
         * @param row Row index [0-8]
         * @param col Column index [0-8]
         */
        void setPawnPosition(int player, int row, int col);

        /**
         * @brief Places a wall on the board.
         * @param wall Wall struct (position and orientation)
         */
        void placeWall(const Wall& wall);

        /**
         * @brief Gets the current position of the specified player's pawn.
         * @param playerIndex Player index (0 or 1)
         * @return Position Pawn's position
         */
        Position getPawnPosition(int playerIndex) const;

        /**
         * @brief Gets all placed walls.
         * @return const std::vector<Wall>& Constant reference to the list of walls
         */
        const std::vector<Wall>& getWalls() const;

        /**
         * @brief Checks if a position is within the 9x9 board boundaries.
         */
        static bool isPositionValid(const Position& pos);

        /**
         * @brief Checks if the given coordinates are within the board bounds.
         * @param row Row index
         * @param col Column index
         * @return true if within bounds, false otherwise
         */
        static bool isInBounds(int row, int col);

    private:
        // 9x9 grid of cells
        std::array<std::array<Cell, BOARD_SIZE>, BOARD_SIZE> grid_;

        // Stores the positions of the two players' pawns
        std::array<Position, NUM_PLAYERS> pawnPositions_; 
        
        // Stores the placed walls
        std::vector<Wall> placedWalls_;

        // Helper method: checks if wall position is within the valid 8x8 wall placement boundaries
        static bool isWallPlacementValid(const Position& pos);
    };

} // namespace Quoridor
