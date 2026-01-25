#pragma once

#include "controller/Scene.hpp"
#include "controller/SceneManager.hpp"
#include <vector>
#include <string>
#include <optional>
#include <filesystem>
#include <SFML/Graphics.hpp>

namespace Quoridor {

class HowToPlayScene : public Scene {
public:
    explicit HowToPlayScene(SceneManager& manager);
    ~HowToPlayScene() override = default;

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render() override;

private:
    struct Page {
        std::string title;
        std::vector<std::string> content;
        std::optional<std::string> imagePath;
    };

    void initializePages();
    std::string resolveImagePath(const std::string& relative) const;
    void nextPage();
    void previousPage();
    void goHome();

    SceneManager& manager;
    Render2D& renderer;
    std::vector<Page> pages;
    size_t currentPageIndex { 0 };
    
    // Image rendering
    sf::Texture imageTexture;
    std::optional<sf::Sprite> imageSprite;
    bool currentImageLoaded { false };
    static constexpr float IMAGE_WIDTH = 500.0f;
    static constexpr float IMAGE_HEIGHT = 480.0f;
};

} // namespace Quoridor
