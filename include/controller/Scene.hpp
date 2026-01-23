#pragma once

#include <SFML/Window/Event.hpp>

namespace Quoridor {

// Base interface for all scenes (Menu, Game, GameOver, etc.)
class Scene {
public:
    virtual ~Scene() = default;

    // Handle a single SFML event (input, window close, etc.)
    virtual void handleEvent(const sf::Event& event) = 0;

    // Per-frame update with delta time in seconds
    virtual void update(float deltaTime) = 0;

    // Per-frame render
    virtual void render() = 0;
};

} // namespace Quoridor
