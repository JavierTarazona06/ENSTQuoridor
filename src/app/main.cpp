#include "view/Render2D.hpp"

int main()
{
    Quoridor::Render2D render;
    Quoridor::Board board;

    // Main loop
    while (render.isOpen())
    {
        // Handle events
        while (const std::optional event = render.pollEvent())
        {
            // If user requests to close the window, it closes
            if (event->is<sf::Event::Closed>())
                render.close();
        }

        // Clear, draw grid, display
        render.clear();
        render.drawGrid();
        render.drawPawn(board, 0); // Draw player 0's pawn for demonstration
        render.drawPawn(board, 1); // Draw player 1's pawn for demonstration
        render.display();
    }

    return 0;
}
