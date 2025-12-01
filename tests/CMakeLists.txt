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
catch_discover_tests(test_all)
