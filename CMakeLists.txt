cmake_minimum_required(VERSION 3.15)

project(Interval VERSION 1.0)

option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/submodules/numeric/submodules/configuration/cmake/ProjectOptions.cmake")
    message(FATAL_ERROR
        "Git submodules are not initialized. "
        "Run 'git submodule update --init --recursive' before configuring the project.")
endif()

include("${PROJECT_SOURCE_DIR}/submodules/numeric/submodules/configuration/cmake/ProjectOptions.cmake")

require_initialized_submodules()
setup_project()

include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/include)

if(NOT TARGET numeric)
    add_subdirectory(submodules)
endif()

add_library(interval INTERFACE)
target_include_directories(interval INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/ariadne-interval>
)
target_link_libraries(interval INTERFACE numeric)

add_subdirectory(src)
target_link_libraries(INTERVAL_SRC PRIVATE interval)

setup_project_library(
    TARGET ariadne-interval
    OBJECTS
        INTERVAL_SRC
        NUMERIC_SRC
        PARADIGM_SRC
        UTILITY_SRC
    LINK_LIBRARIES interval
)

setup_standalone_project_tests(
    TARGET ariadne-interval
    TEST_DIRECTORY tests
    TEST_TARGET tests
    SOURCES
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/src"
)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    install(FILES "${PROJECT_SOURCE_DIR}/include/ariadne-interval.hpp"
        DESTINATION include/ariadne-interval
    )
    install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/interval"
        DESTINATION include/ariadne-interval
    )
endif()
