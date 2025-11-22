#include <catch2/catch_test_macros.hpp>
#include "view/Render2D.hpp"
#include <iostream>

using namespace Quoridor;

TEST_CASE("Render2D font loading", "[view][fonts]") {
    SECTION("All fonts should load successfully") {
        Render2D render;
        
        // If the constructor completes without throwing, fonts loaded
        // We can verify by checking if the window was created successfully
        REQUIRE(render.isOpen() == true);
        
        // Close the window to clean up
        render.close();
        REQUIRE(render.isOpen() == false);
    }
    
    SECTION("Window dimensions should be correct") {
        REQUIRE(Render2D::getWindowWidth() == 900);
        REQUIRE(Render2D::getWindowHeight() == 900);
    }
    
    SECTION("Grid offset values should be correct") {
        REQUIRE(Render2D::getGridOffsetX() == 180.0f);
        REQUIRE(Render2D::getGridOffsetY() == 180.0f);
    }
}

TEST_CASE("Render2D text rendering with different fonts", "[view][fonts][text]") {
    SECTION("Draw text with all font types") {
        Render2D render;
        Board board;
        
        // Test drawing text with each font type (0-3)
        // This should not crash if fonts are loaded correctly
        render.clear();
        
        // Title1 (ArialCEMTBlack.ttf)
        render.drawText("Title 1", 450.0f, 50.0f, 48, sf::Color::White, 0);
        
        // Title2 (ArialCEBoldItalic.ttf)
        render.drawText("Title 2", 450.0f, 120.0f, 36, sf::Color::Yellow, 1);
        
        // Title3 (ARIALBD.TTF)
        render.drawText("Title 3", 450.0f, 180.0f, 24, sf::Color::Green, 2);
        
        // Text (ARIAL.TTF) - default
        render.drawText("Normal Text", 450.0f, 240.0f, 18, sf::Color::White, 3);
        
        // Default font (should use fontText)
        render.drawText("Default", 450.0f, 280.0f, 18, sf::Color::White);
        
        render.display();
        
        // If we reached here without crashing, fonts work
        REQUIRE(render.isOpen() == true);
        
        render.close();
    }
}
