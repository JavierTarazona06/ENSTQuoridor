cmake_minimum_required(VERSION 3.20)
project(ENSTQuoridor VERSION 1.0 LANGUAGES CXX)

# Uses C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML package
find_package(SFML COMPONENTS Graphics Window System REQUIRED)

# Create executable
add_executable(hello_sfml src/app/main.cpp)

# Link SFML libraries
target_link_libraries(hello_sfml PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System
)