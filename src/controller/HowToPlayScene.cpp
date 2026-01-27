#include "controller/HowToPlayScene.hpp"
#include "controller/MenuScene.hpp"

namespace Quoridor {

HowToPlayScene::HowToPlayScene(SceneManager& manager)
    : manager(manager), renderer(manager.getRenderer()) {
    initializePages();
}

void HowToPlayScene::initializePages() {
    pages.clear();

    // Page 1 - Core Rules
    pages.push_back({
        "Core Rules",
        {
            "Objective: Reach the opposite row",
            "before your opponent."
        },
        resolveImagePath("assets/img/tutorial1.jpg")
    });

    // Page 2 - Turn options
    pages.push_back({
        "Turn Options",
        {
            "Move pawn: One cell orthogonally",
            "(up, down, left, right).",
            "",
            "Place wall: Between two cells;",
            "consumes one of your walls."
        },
        resolveImagePath("assets/img/tutorial2.png")
    });

    // Page 3 - Jumping
    pages.push_back({
        "Jumping",
        {
            "If the opponent is directly adjacent you may jump ",
            "over them if the square behind them is free.",
            "",
            "If the straight jump is blocked or off-board, a",
            "diagonal jump to one of the two adjacent diagonals is allowed."
        },
        resolveImagePath("assets/img/tutorial3.png")
    });

    // Page 4 - Wall constraint / Inventory
    pages.push_back({
        "Wall Constraint & Inventory",
        {
            "Wall constraint: A wall must not completely block any player's path to",
            "their goal at least one path must remain.",
            "",
            "Inventory: Each player starts with",
            "10 walls."
        },
        resolveImagePath("assets/img/tutorial4.png")
    });

    // Page 5 - Victory
    pages.push_back({
        "Victory",
        {
            "First to reach their goal row wins.",
            "",
            "Player 1 : top row (red)",
            "Player 2 : bottom row (blue)"
        },
        resolveImagePath("assets/img/tutorial5.png")
    });

    // Page 6 - Game Modes
    pages.push_back({
        "Game Modes",
        {
            "Human vs Human: Local multiplayer",
            "on the same machine",
            "",
            "Human vs AI: Challenge the computer",
            "at four difficulty levels"
        },
        resolveImagePath("assets/img/tutorial6.png")
    });

    // Page 7 - Controls Menu
    pages.push_back({
        "Controls - Menu",
        {
            "H - Human vs Human mode",
            "A - Human vs AI mode",
            "1/2/3/4 - Easy/Normal/Hard/Hell AI difficulty",
            "Enter - Start game",
            "Esc - Quit"
        },
        resolveImagePath("assets/img/tutorial7.png")
    });

    // Page 8 - Controls In-Game
    pages.push_back({
        "Controls - In-Game",
        {
            "Mouse: Click pawn to select, then click destination to move. Click selected pawn again to deselect.",
            "W - Toggle wall placement mode",
            "O - Rotate wall (horizontal <-> vertical)",
            "1/2/3/4 - Change AI difficulty",
            "R - Reset game | Esc - Return to menu"
        },
        resolveImagePath("assets/img/tutorial8.png")
    });
}

std::string HowToPlayScene::resolveImagePath(const std::string& relative) const {
    namespace fs = std::filesystem;

    // 1) Try runtime path relative to working directory (packaged assets next to exe)
    try {
        fs::path cwd = fs::current_path();
        fs::path candidate = cwd / relative;
        if (fs::exists(candidate)) {
            return candidate.generic_string();
        }
    } catch (...) {
        // ignore
    }

    // 2) Fallback to developer tree based on FONT_DIR (same strategy as Render2D)
    try {
        fs::path devRoot = fs::path(std::string(FONT_DIR)).parent_path();
        fs::path candidate = devRoot / fs::path(relative);
        if (fs::exists(candidate)) {
            return candidate.generic_string();
        }
    } catch (...) {
        // ignore
    }

    // 3) Last resort: return relative so SFML error is logged if missing
    return relative;
}

void HowToPlayScene::nextPage() {
    if (currentPageIndex < pages.size() - 1) {
        currentPageIndex++;
        currentImageLoaded = false;  // Reset image load state
    }
}

void HowToPlayScene::previousPage() {
    if (currentPageIndex > 0) {
        currentPageIndex--;
        currentImageLoaded = false;  // Reset image load state
    }
}

void HowToPlayScene::goHome() {
    manager.setScene(std::make_unique<MenuScene>(manager), GameState::Menu);
}

void HowToPlayScene::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        renderer.close();
        return;
    }

    if (event.is<sf::Event::KeyPressed>()) {
        const auto key = event.getIf<sf::Event::KeyPressed>()->code;

        // Right arrow or D - Next page
        if (key == sf::Keyboard::Key::Right || key == sf::Keyboard::Key::D) {
            nextPage();
            return;
        }

        // Left arrow or A - Previous page
        if (key == sf::Keyboard::Key::Left || key == sf::Keyboard::Key::A) {
            previousPage();
            return;
        }

        // Escape - Go back to menu
        if (key == sf::Keyboard::Key::Escape) {
            goHome();
            return;
        }

        // H - Go back to menu
        if (key == sf::Keyboard::Key::H) {
            goHome();
            return;
        }
    }

    // Mouse click detection
    if (event.is<sf::Event::MouseButtonPressed>()) {
        const auto& mouse = event.getIf<sf::Event::MouseButtonPressed>()->position;
        const float centerX = Render2D::getWindowWidth() / 2.0f;
        const float centerY = Render2D::getWindowHeight() / 2.0f;
        const float buttonWidth = 100.0f;
        const float buttonHeight = 35.0f;

        // Home button (top-right corner)
        const float homeX = Render2D::getWindowWidth() - 70.0f;
        const float homeY = 20.0f;
        const bool overHome = (mouse.x >= homeX - buttonWidth / 2.0f &&
                               mouse.x <= homeX + buttonWidth / 2.0f &&
                               mouse.y >= homeY - buttonHeight / 2.0f &&
                               mouse.y <= homeY + buttonHeight / 2.0f);

        if (overHome) {
            goHome();
            return;
        }

        // Previous button (bottom-left)
        const float prevX = 90.0f;
        const float prevY = Render2D::getWindowHeight() - 30.0f;
        const bool overPrev = (mouse.x >= prevX - buttonWidth / 2.0f &&
                               mouse.x <= prevX + buttonWidth / 2.0f &&
                               mouse.y >= prevY - buttonHeight / 2.0f &&
                               mouse.y <= prevY + buttonHeight / 2.0f);

        if (overPrev) {
            previousPage();
            return;
        }

        // Next button (bottom-right)
        const float nextX = Render2D::getWindowWidth() - 90.0f;
        const float nextY = Render2D::getWindowHeight() - 30.0f;
        const bool overNext = (mouse.x >= nextX - buttonWidth / 2.0f &&
                               mouse.x <= nextX + buttonWidth / 2.0f &&
                               mouse.y >= nextY - buttonHeight / 2.0f &&
                               mouse.y <= nextY + buttonHeight / 2.0f);

        if (overNext) {
            nextPage();
            return;
        }
    }
}

void HowToPlayScene::update(float deltaTime) {
    (void)deltaTime; // Unused
}

void HowToPlayScene::render() {
    renderer.clear();

    const float centerX = Render2D::getWindowWidth() / 2.0f;
    const float centerY = Render2D::getWindowHeight() / 2.0f;

    // Title
    renderer.drawText("How to Play", centerX, 50.0f, 40, sf::Color::White, 1);

    // Home button (top-right)
    const float homeX = Render2D::getWindowWidth() - 70.0f;
    renderer.drawText("[HOME]", homeX, 30.0f, 18, sf::Color(150, 200, 255), 3);

    // Page title
    if (currentPageIndex < pages.size()) {
        const auto& page = pages[currentPageIndex];
        renderer.drawText(page.title, centerX, 120.0f, 32, sf::Color(200, 220, 255), 2);

        // Page content (text always starts at same position)
        float contentY = 180.0f;
        const float lineSpacing = 35.0f;
        for (const auto& line : page.content) {
            if (!line.empty()) {
                renderer.drawText(line, centerX, contentY, 20, sf::Color(220, 220, 220), 2);
            }
            contentY += lineSpacing;
        }

        // Load and display image below the text if available
        if (page.imagePath.has_value() && !currentImageLoaded) {
            if (imageTexture.loadFromFile(page.imagePath.value())) {
                imageSprite.emplace(imageTexture);
                // Scale to fixed size
                const sf::Vector2u texSize = imageTexture.getSize();
                const float scaleX = IMAGE_WIDTH / texSize.x;
                const float scaleY = IMAGE_HEIGHT / texSize.y;
                imageSprite->setScale({scaleX, scaleY});
                // Center the image horizontally, place below text with some spacing
                imageSprite->setPosition({centerX - IMAGE_WIDTH / 2.0f, contentY + 20.0f});
                currentImageLoaded = true;
            } else {
                imageSprite.reset();
                currentImageLoaded = true;  // Mark as attempted to avoid repeated tries
            }
        }

        // Draw the image if loaded (after text)
        if (page.imagePath.has_value() && imageSprite.has_value()) {
            renderer.getWindow().draw(*imageSprite);
        }
    }

    // Page counter
    std::string pageCounter = std::to_string(currentPageIndex + 1) + " / " + std::to_string(pages.size());
    renderer.drawText(pageCounter, centerX, Render2D::getWindowHeight() - 30.0f, 18, sf::Color(150, 150, 150), 3);

    // Previous button (bottom-left)
    sf::Color prevColor = (currentPageIndex > 0) ? sf::Color(100, 255, 100) : sf::Color(100, 100, 100);
    renderer.drawText("[PREV]", 90.0f, Render2D::getWindowHeight() - 30.0f, 18, prevColor, 3);

    // Next button (bottom-right)
    sf::Color nextColor = (currentPageIndex < pages.size() - 1) ? sf::Color(100, 255, 100) : sf::Color(100, 100, 100);
    renderer.drawText("[NEXT]", Render2D::getWindowWidth() - 90.0f, Render2D::getWindowHeight() - 30.0f, 18, nextColor, 3);

    renderer.display();
}

} // namespace Quoridor
