# Preferable to avoid warnings during execution
cmake_minimum_required(VERSION 3.15)

project(TutorialInterval VERSION 1.0)

# Set the C++ standard to C++20, as used by Ariadne Interval
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the standalone Ariadne Interval installation through the local module
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
find_package(AriadneInterval REQUIRED)

# Build the standalone tutorial
add_executable(tutorial_interval tutorial_interval.cpp)

# Link only against the installed Ariadne Interval library and its numeric dependencies
target_link_libraries(tutorial_interval PRIVATE AriadneInterval::ariadne-interval)
