#include "view/Button.hpp"

namespace Quoridor {

Button::Button(float x, float y, float width, float height, const std::string& text, unsigned int fontSize)
    : text(text), fontSize(fontSize), hovering(false) {
    shape.setSize({width, height});
    shape.setPosition({x, y});
    shape.setFillColor(IDLE_COLOR);
    shape.setOutlineColor(BORDER_COLOR);
    shape.setOutlineThickness(BORDER_THICKNESS);
}

bool Button::isPointInside(float x, float y) const {
    return shape.getGlobalBounds().contains({x, y});
}

void Button::updateHover(float x, float y) {
    bool wasHovering = hovering;
    hovering = isPointInside(x, y);
    
    if (hovering && !wasHovering) {
        shape.setFillColor(HOVER_COLOR);
        shape.setOutlineColor(HOVER_BORDER_COLOR);
    } else if (!hovering && wasHovering) {
        shape.setFillColor(IDLE_COLOR);
        shape.setOutlineColor(BORDER_COLOR);
    }
}

bool Button::isHovering() const {
    return hovering;
}

void Button::setOnClick(std::function<void()> callback) {
    onClick = callback;
}

bool Button::handleClick(float x, float y) {
    if (isPointInside(x, y)) {
        if (onClick) {
            onClick();
        }
        return true;
    }
    return false;
}

void Button::draw(sf::RenderWindow& window, const sf::Font& font, const sf::Color& textColor) {
    // Draw button background and border
    window.draw(shape);

    // Draw button text (centered)
    sf::Text buttonText(font, text, fontSize);
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    
    float centerX = shape.getPosition().x + shape.getSize().x / 2.0f;
    float centerY = (shape.getPosition().y + shape.getSize().y / 2.0f) - 3.0f;
    
    buttonText.setOrigin({textBounds.size.x / 2.0f, textBounds.size.y / 2.0f});
    buttonText.setPosition({centerX, centerY});
    buttonText.setFillColor(textColor);
    
    window.draw(buttonText);
}

void Button::setPosition(float x, float y) {
    shape.setPosition({x, y});
}

sf::FloatRect Button::getBounds() const {
    return shape.getGlobalBounds();
}

} // namespace Quoridor
