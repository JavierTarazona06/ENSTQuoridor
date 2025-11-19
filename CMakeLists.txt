cmake_minimum_required(VERSION 3.20)
project(ENSTQuoridor VERSION 1.0 LANGUAGES CXX)

# Uses C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Find SFML package
find_package(SFML COMPONENTS Graphics Window System REQUIRED)

# Library for the model
add_library(quoridor_model 
    src/model/Board.cpp
    src/model/State.cpp
    src/model/Rules.cpp
)

# Create executable for the game
add_executable(quoridor_game src/app/main.cpp)
target_link_libraries(quoridor_game PRIVATE 
    quoridor_model
    SFML::Graphics 
    SFML::Window 
    SFML::System
)

# Create executable for tests
add_executable(test_board tests/test_board.cpp)
target_link_libraries(test_board PRIVATE 
    quoridor_model
)

add_executable(test_state tests/test_state.cpp)
target_link_libraries(test_state PRIVATE 
    quoridor_model
)

add_executable(test_rules tests/test_rules.cpp)
target_link_libraries(test_rules PRIVATE 
    quoridor_model
)
