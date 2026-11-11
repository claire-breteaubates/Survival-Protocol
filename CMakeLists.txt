
cmake_minimum_required(VERSION 3.28)

project(SurvivalProtocol LANGUAGES CXX)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build SFML as static libraries to avoid SFML DLL setup
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Download SFML automatically
include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(SFML)

# Define our game executable
add_executable(SurvivalProtocol
    SFML_Week5/main.cpp
    SFML_Week5/Game.cpp
    SFML_Week5/Player.cpp
    SFML_Week5/Enemy.cpp
    SFML_Week5/Projectile.cpp
    SFML_Week5/WaveManager.cpp
    SFML_Week5/EnemyConfig.cpp
)

# Link the SFML Graphics module and its dependencies
target_link_libraries(SurvivalProtocol
    PRIVATE SFML::Graphics
)

# Copy assets next to the executable
add_custom_command(
    TARGET SurvivalProtocol POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/SFML_Week5/assets"
        "$<TARGET_FILE_DIR:SurvivalProtocol>/assets"
    COMMENT "Copying game assets"
)

# Start the game in the executable directory
set_target_properties(SurvivalProtocol PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY
    "$<TARGET_FILE_DIR:SurvivalProtocol>"
)

# Set Visual Studio's startup project
set_property(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT SurvivalProtocol
)
