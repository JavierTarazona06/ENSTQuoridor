cmake_minimum_required(VERSION 3.20)
project(ENSTQuoridor VERSION 1.0 LANGUAGES CXX)

# Uses C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML package
find_package(SFML COMPONENTS Graphics Window System REQUIRED)

# Library for the model
add_library(quoridor_model 
    src/model/Board.cpp
    src/model/Pathfinder.cpp
    src/model/State.cpp
    src/model/Rules.cpp
    src/view/Render2D.cpp
    src/view/GameView.cpp
    src/controller/server.cpp
    src/controller/InputHandler.cpp
    src/controller/Game.cpp
)
# Define FONT_DIR for font assets
target_compile_definitions(quoridor_model PUBLIC FONT_DIR=\"${CMAKE_SOURCE_DIR}/assets/fonts\")

target_include_directories(quoridor_model PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(quoridor_model PUBLIC 
    SFML::Graphics 
    SFML::Window 
    SFML::System
)

# Create executable for the game
add_executable(quoridor_game src/app/main.cpp)
target_link_libraries(quoridor_game PRIVATE 
    quoridor_model
    SFML::Graphics 
    SFML::Window 
    SFML::System
)

# Add tests directory
add_subdirectory(tests)
