# Tests CMakeLists.txt for ENSTQuoridor

enable_testing()

# Find Catch2 package
find_package(Catch2 3 REQUIRED CONFIG)

# Create a single test executable with all test files
add_executable(test_all
    test_board.cpp
    test_state.cpp
    test_rules.cpp
    test_walls.cpp
    test_view.cpp
    test_input_handler.cpp
    test_jumps.cpp
    test_pathfinder.cpp
    test_ai_core.cpp
    test_heuristics.cpp
    test_minimax.cpp
)

target_link_libraries(test_all 
    PRIVATE 
        quoridor_model
        Catch2::Catch2WithMain
)

# Automatically discover and register all Catch2 tests with CTest
include(CTest)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(test_all EXTRA_ARGS --benchmark-samples 10)

# ============================================================================
# PVE Visual Test - Interactive game with AI opponent
# This uses the existing UI components but adds AI control
# ============================================================================
add_executable(test_pve_visual
    test_pve_visual.cpp
)

target_link_libraries(test_pve_visual 
    PRIVATE 
        quoridor_model
        SFML::Graphics
        SFML::Window
        SFML::System
)

# Link to the view library components (reuse from main project)
target_include_directories(test_pve_visual PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Need to compile view sources for this test
target_sources(test_pve_visual PRIVATE
    ${CMAKE_SOURCE_DIR}/src/view/Render2D.cpp
    ${CMAKE_SOURCE_DIR}/src/view/Button.cpp
    ${CMAKE_SOURCE_DIR}/src/view/GameView.cpp
)
