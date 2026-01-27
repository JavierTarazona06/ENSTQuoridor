#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

namespace Quoridor {

/**
 * @brief Simple button UI component for game UI
 * 
 * Provides hover effects, click detection, and text rendering.
 * Designed to integrate with SFML-based rendering pipeline.
 */
class Button {
public:
    /**
     * @brief Create a button with text label
     * @param x Position X
     * @param y Position Y
     * @param width Button width
     * @param height Button height
     * @param text Button label
     * @param fontSize Font size for text
     */
    Button(float x, float y, float width, float height, const std::string& text, unsigned int fontSize = 18);

    /**
     * @brief Check if a point is inside the button
     * @param x Point X coordinate
     * @param y Point Y coordinate
     * @return True if point is inside button bounds
     */
    bool isPointInside(float x, float y) const;

    /**
     * @brief Update hover state based on mouse position
     * @param x Mouse X coordinate
     * @param y Mouse Y coordinate
     */
    void updateHover(float x, float y);

    /**
     * @brief Check if mouse is hovering over the button
     * @return True if mouse is hovering
     */
    bool isHovering() const;

    /**
     * @brief Set callback to execute on click
     * @param callback Function to call when button is clicked
     */
    void setOnClick(std::function<void()> callback);

    /**
     * @brief Handle mouse click
     * @param x Click X coordinate
     * @param y Click Y coordinate
     * @return True if button was clicked
     */
    bool handleClick(float x, float y);

    /**
     * @brief Draw button on window
     * @param window SFML render window
     * @param font Font for text rendering
     * @param color Text color (default white)
     */
    void draw(sf::RenderWindow& window, const sf::Font& font, const sf::Color& textColor = sf::Color::White);

    /**
     * @brief Set button position
     * @param x New X position
     * @param y New Y position
     */
    void setPosition(float x, float y);

    /**
     * @brief Get button bounds
     * @return Rectangle bounds
     */
    sf::FloatRect getBounds() const;

private:
    sf::RectangleShape shape;
    std::string text;
    unsigned int fontSize;
    bool hovering;
    std::function<void()> onClick;

    // Styling constants
    static constexpr float BORDER_THICKNESS = 2.0f;
    static constexpr sf::Color IDLE_COLOR = sf::Color(50, 50, 50);
    static constexpr sf::Color HOVER_COLOR = sf::Color(80, 80, 80);
    static constexpr sf::Color BORDER_COLOR = sf::Color(200, 200, 200);
    static constexpr sf::Color HOVER_BORDER_COLOR = sf::Color(255, 255, 255);
};

} // namespace Quoridor
