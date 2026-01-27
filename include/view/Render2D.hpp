#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include <optional>
#include "model/Board.hpp"
#include "model/State.hpp"

namespace Quoridor {

class Render2D {
public:
    Render2D();
    ~Render2D();

    // Window management
    bool isOpen() const;
    void close();
    void clear();
    void display();

    // Event handling
    std::optional<sf::Event> pollEvent();

    // Rendering methods
    void drawGrid();
    void drawPawn(const Board& board, int playerNumber);
    void drawText(const std::string& text, float x, float y, unsigned int fontSize, const sf::Color& color, int fontType = 3);
    void drawWalls(const Board& board);
    void drawWallPreview(const std::optional<Wall>& previewWall);
    void drawHUD(const Board& board, const State& state);
    void drawButton(class Button& button);  // Forward declaration

    // Message box system
    void showMessage(const std::string& text, const Color& color, float duration = -1.0f);
    void drawMessage();
    void updateMessage(float deltaTime);

    // Logo rendering with optional custom position
    void drawLogo(std::optional<float> logoX = std::nullopt, std::optional<float> logoY = std::nullopt);

    // Font management
    bool loadFont(const std::string& fontPath, int fontType);

    // Get window reference
    sf::RenderWindow& getWindow();
    
    // Enforce fixed window size (prevents DPI scaling when moving between monitors)
    void enforceFixedSize();

    // Getters for constants
    static constexpr unsigned int getWindowWidth() { return WINDOW_WIDTH; }
    static constexpr unsigned int getWindowHeight() { return WINDOW_HEIGHT; }
    static constexpr float getGridOffsetX() { return GRID_OFFSET_X; }
    static constexpr float getGridOffsetY() { return GRID_OFFSET_Y; }
    static constexpr float getCellSize() { return CELL_SIZE; }

private:
    sf::RenderWindow window;
    std::string fontsDir;
    std::string assetsDir;
    sf::Font fontTitle1;
    sf::Font fontTitle2;
    sf::Font fontTitle3;
    sf::Font fontText;

    // Message box state
    std::string currentMessageText;
    sf::Color currentMessageColor;
    float messageDuration;
    float messageElapsed;
    bool messageActive;

    // Background message state (for preemption)
    std::string backgroundMessageText;
    sf::Color backgroundMessageColor;
    bool hasBackgroundMessage;

    // Logo assets
    sf::Texture logoTexture;
    std::optional<sf::Sprite> logoSprite;
    bool logoLoaded;

    static constexpr unsigned int WINDOW_WIDTH = 1050;
    static constexpr unsigned int WINDOW_HEIGHT = 900;
    static constexpr float CELL_SIZE = 60.0f;
    static constexpr float GRID_OFFSET_X = WINDOW_WIDTH / 2.0f - (CELL_SIZE * BOARD_SIZE) / 2.0f; // Center the grid
    //180.0f; //TODO/ Delete
    static constexpr float GRID_OFFSET_Y = 180.0f;
    static constexpr float PAWN_RADIUS = 20.0f;
    static constexpr sf::Color BOARD_COLOR = sf::Color(240, 217, 181); //Beige

    // Wall constants
    static constexpr float WALL_THICKNESS = 6.0f;  // Wall thickness (~3px on each side)
    static constexpr sf::Color WALL_COLOR = sf::Color(139, 69, 19); // Brown (SaddleBrown)
    static constexpr sf::Color WALL_OUTLINE_COLOR = sf::Color(80, 40, 10); // Darker outline
    static constexpr sf::Color WALL_PREVIEW_COLOR = sf::Color(139, 69, 19, 100); // Semi-transparent preview
    
    // HUD constants
    static constexpr float HUD_BOX_WIDTH = 160.0f;
    static constexpr float HUD_BOX_HEIGHT = 220.0f;
    static constexpr sf::Color HUD_BACKGROUND_COLOR = sf::Color(40, 40, 40); // Dark gray
    static constexpr sf::Color HUD_BORDER_COLOR = sf::Color::White;
    static constexpr float HUD_BORDER_THICKNESS = 2.0f;

    // Message box constants
    static constexpr float MESSAGE_BOX_WIDTH = 800.0f;
    static constexpr float MESSAGE_BOX_HEIGHT = 100.0f;
    static constexpr float MESSAGE_BOX_MARGIN_BOTTOM = 20.0f;
    static constexpr unsigned int MESSAGE_FONT_SIZE = 24;

    // Resolve fonts directory at runtime (package vs dev tree)
    std::string resolveFontsDir() const;
    std::string resolveAssetsDir(const std::string& resolvedFontsDir) const;
};

} // namespace Quoridor
