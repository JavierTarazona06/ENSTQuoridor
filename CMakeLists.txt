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

# Find SFML package (version 3 required for vcpkg SFML 3.x)
find_package(SFML 3 COMPONENTS ${SFML_COMPONENTS} REQUIRED)

# Library for the model
add_library(quoridor_model 
    src/model/Board.cpp
    src/model/Pathfinder.cpp
    src/model/State.cpp
    src/model/Rules.cpp
    src/model/AI.cpp
    src/view/Render2D.cpp
    src/view/Button.cpp
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

# Create executable for the game
# - WIN32 hides console on Windows
# - MACOSX_BUNDLE creates .app bundle on macOS
if(APPLE)
    add_executable(quoridor_game MACOSX_BUNDLE src/app/main.cpp)
    
    # Configure macOS bundle properties
    set_target_properties(quoridor_game PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "Quoridor"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.ensta.quoridor"
        MACOSX_BUNDLE_ICON_FILE "quoridor.icns"
        MACOSX_BUNDLE_COPYRIGHT "© 2026 ENSTA Paris"
        MACOSX_BUNDLE_INFO_STRING "Quoridor Board Game"
    )
elseif(WIN32)
    add_executable(quoridor_game WIN32 src/app/main.cpp)
else()
    add_executable(quoridor_game src/app/main.cpp)
endif()

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
# Platform-specific installation destinations
if(APPLE)
    # macOS: Install bundle to Applications-like location
    set(QUORIDOR_RUNTIME_DEST ".")
    set(QUORIDOR_BUNDLE_DEST ".")
    
    # Install the .app bundle
    install(TARGETS quoridor_game
        BUNDLE DESTINATION ${QUORIDOR_BUNDLE_DEST}
        RUNTIME DESTINATION ${QUORIDOR_RUNTIME_DEST}
    )
    
    # Copy assets into the bundle's Resources folder
    install(DIRECTORY assets/
        DESTINATION "quoridor_game.app/Contents/Resources/assets"
    )
    
elseif(WIN32)
    set(QUORIDOR_RUNTIME_DEST ".")
    install(TARGETS quoridor_game RUNTIME DESTINATION ${QUORIDOR_RUNTIME_DEST})
    install(DIRECTORY assets/ DESTINATION assets)
else()
    set(QUORIDOR_RUNTIME_DEST "bin")
    install(TARGETS quoridor_game RUNTIME DESTINATION ${QUORIDOR_RUNTIME_DEST})
    install(DIRECTORY assets/ DESTINATION assets)
endif()

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
set(CPACK_PACKAGE_NAME "Quoridor")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "ENSTA Paris")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Quoridor Board Game")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/README.md")

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
    set(CPACK_PACKAGE_FILE_NAME "Quoridor-${PROJECT_VERSION}-Windows-x64")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_PACKAGE_FILE_NAME "Quoridor-${PROJECT_VERSION}-macOS")
    
    # DMG configuration
    set(CPACK_DMG_VOLUME_NAME "Quoridor")
    set(CPACK_DMG_FORMAT "UDBZ")  # Compressed format
    
    # Optional: Custom DMG background (uncomment if you have one)
    # set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_SOURCE_DIR}/assets/img/dmg_background.png")
    
    # Bundle configuration for CPack
    set(CPACK_BUNDLE_NAME "Quoridor")
    set(CPACK_BUNDLE_PLIST "${CMAKE_BINARY_DIR}/Info.plist")
    set(CPACK_BUNDLE_ICON "${CMAKE_SOURCE_DIR}/assets/img/quoridor.icns")
else()
    set(CPACK_GENERATOR "TGZ")
    set(CPACK_PACKAGE_FILE_NAME "Quoridor-${PROJECT_VERSION}-Linux-x64")
endif()

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)  # No extra wrapper folder in archives

include(CPack)
