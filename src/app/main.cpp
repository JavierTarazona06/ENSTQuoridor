#include "view/Render2D.hpp"

int main()
{
    Quoridor::Render2D render;

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
        render.display();
    }

    return 0;
}
