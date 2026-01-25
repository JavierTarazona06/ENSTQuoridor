cmake_minimum_required(VERSION 3.20)
project(ENSTQuoridor VERSION 1.0 LANGUAGES CXX)

# Force SFML shared libraries on Unix to avoid missing static config with vcpkg default triplet
set(SFML_STATIC_LIBRARIES OFF CACHE BOOL "Use SFML shared libraries")

if (WIN32)
  set(SFML_COMPONENTS Graphics Window System Main)
else()
  set(SFML_COMPONENTS Graphics Window System Audio Network)
endif()

# Uses C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile commands for IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SFML package
find_package(SFML COMPONENTS ${SFML_COMPONENTS} REQUIRED)

# Library for the model
add_library(quoridor_model 
    src/model/Board.cpp
    src/model/Pathfinder.cpp
    src/model/State.cpp
    src/model/Rules.cpp
    src/model/AI.cpp
    src/view/Render2D.cpp
    src/view/GameView.cpp
    src/controller/server.cpp
    src/controller/InputHandler.cpp
    src/controller/SceneManager.cpp
    src/controller/MenuScene.cpp
    src/controller/GameScene.cpp
    src/controller/GameOverScene.cpp
    src/controller/HowToPlayScene.cpp
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

# Create executable for the game (WIN32 hides console on Windows)
add_executable(quoridor_game WIN32 src/app/main.cpp)
target_link_libraries(quoridor_game PRIVATE 
    quoridor_model
    SFML::Graphics 
    SFML::Window 
    SFML::System
)

# On Windows, link SFML::Main to provide WinMain wrapper for main()
if(WIN32)
    target_link_libraries(quoridor_game PRIVATE SFML::Main)
endif()

# Add tests directory
add_subdirectory(tests)

# ===== INSTALL RULES =====
# Install executable (bin on Unix, root on Windows for legacy ZIP layout)
if (WIN32)
  set(QUORIDOR_RUNTIME_DEST ".")
else()
  set(QUORIDOR_RUNTIME_DEST "bin")
endif()
install(TARGETS quoridor_game RUNTIME DESTINATION ${QUORIDOR_RUNTIME_DEST})

# Install assets folder alongside executable
install(DIRECTORY assets/ DESTINATION assets)

# ===== INSTALL DEPENDENCIES FOR WINDOWS =====
# Manually copy SFML and runtime DLLs needed for portable package
if(WIN32)
  # Get SFML binary directory from vcpkg
  get_target_property(SFML_GRAPHICS_LIB SFML::Graphics IMPORTED_LOCATION_RELEASE)
  get_filename_component(SFML_BIN_DIR "${SFML_GRAPHICS_LIB}" DIRECTORY)
  
  # Install SFML DLLs
  install(FILES
    "${SFML_BIN_DIR}/sfml-graphics-3.dll"
    "${SFML_BIN_DIR}/sfml-window-3.dll"
    "${SFML_BIN_DIR}/sfml-system-3.dll"
    "${SFML_BIN_DIR}/sfml-audio-3.dll"
    DESTINATION .
    OPTIONAL  # in case some DLLs aren't available
  )
  
  # Install OpenAL (usually needed by SFML audio)
  install(FILES
    "${SFML_BIN_DIR}/OpenAL32.dll"
    DESTINATION .
    OPTIONAL
  )

  # Also copy all dependency DLLs from vcpkg's bin (freetype, libpng, zlib, brotli, bzip2, etc.)
  # This ensures users won't miss transitive dependencies on fresh machines.
  if (VCPKG_TARGET_TRIPLET)
    set(VCPKG_BIN_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/bin")
    if (EXISTS "${VCPKG_BIN_DIR}")
      install(DIRECTORY "${VCPKG_BIN_DIR}/" DESTINATION . FILES_MATCHING PATTERN "*.dll")
    endif()
  endif()
  
  # Optional: Install MSVC runtime (uncomment if needed)
  include(InstallRequiredSystemLibraries)
endif()

# ===== CPACK CONFIGURATION =====
if (WIN32)
  set(CPACK_GENERATOR "ZIP")
  set(CPACK_PACKAGE_FILE_NAME "Quoridor-1.0-Windows")
elseif(UNIX AND NOT APPLE)
  set(CPACK_GENERATOR "TGZ")
  set(CPACK_PACKAGE_FILE_NAME "Quoridor-1.0-Linux")
else()
  set(CPACK_GENERATOR "DragNDrop")
  set(CPACK_PACKAGE_FILE_NAME "Quoridor-1.0-macOS")
endif()

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)  # No extra wrapper folder in archives

include(CPack)
