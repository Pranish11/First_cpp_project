cmake_minimum_required(VERSION 3.28)

# Project name and language
project(Project_1 LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent for SFML
include(FetchContent)

# Download and build SFML 3.0.2
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(SFML)

# Define your executable target
add_executable(main main.cpp)

# Link SFML Graphics module
target_link_libraries(main PRIVATE SFML::Graphics)
