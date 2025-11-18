#include <SFML/Graphics.hpp>

int main()
{
    // Create a window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Hello SFML!");

    // Create a circle shape
    sf::CircleShape circle(50.f); // Radius of 50 pixels
    circle.setFillColor(sf::Color::Green);
    circle.setPosition({375.f, 275.f}); // 0,0 is at upper left corner

    // Main loop
    while (window.isOpen())
    {
        // Handle events
        while (const std::optional event = window.pollEvent())
        {
            // If user requests to close the window, it closes (event)
            if (event -> is<sf::Event::Closed>())
                window.close();
        }

        // Clear, draw, display
        window.clear(sf::Color::Black);
        window.draw(circle);
        window.display();
    }

    return 0;
}
