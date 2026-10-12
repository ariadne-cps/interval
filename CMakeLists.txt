cmake_minimum_required(VERSION 3.15)

project(Interval VERSION 1.0)

option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(THREAD_SANITIZER "Enable ThreadSanitizer" OFF)
option(BUILD_PYTHON_BINDINGS "Build Python bindings" ON)

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/ss/configuration/cmake/ProjectOptions.cmake")
    message(FATAL_ERROR
        "Git submodules are not initialized. "
        "Run 'git submodule update --init --recursive' before configuring the project.")
endif()

include("${PROJECT_SOURCE_DIR}/ss/configuration/cmake/ProjectOptions.cmake")

require_initialized_submodules()
setup_project()

if(COVERAGE AND THREAD_SANITIZER)
    message(FATAL_ERROR "COVERAGE and THREAD_SANITIZER cannot be enabled together.")
endif()

if(THREAD_SANITIZER AND CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    if(WIN32 OR NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        message(FATAL_ERROR "ThreadSanitizer requires GCC or Clang on Unix-like systems.")
    endif()
    add_compile_options(-fsanitize=thread -fno-omit-frame-pointer)
    add_link_options(-fsanitize=thread)
endif()

if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
    require_same_dependency_commit(Configuration
        REFERENCES
            "${PROJECT_SOURCE_DIR}|ss/configuration"
            "${PROJECT_SOURCE_DIR}/ss/numeric|ss/configuration"
    )
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

if(NOT TARGET numeric)
    add_subdirectory(ss)
endif()

add_library(interval INTERFACE)
target_include_directories(interval INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/ariadne-interval>
)
target_link_libraries(interval INTERFACE numeric)

add_subdirectory(src)
target_link_libraries(INTERVAL_SRC PRIVATE interval)

ariadne_register_public_headers(
    TARGET interval
    DIRECTORIES "${PROJECT_SOURCE_DIR}/include/interval"
    FILES "${PROJECT_SOURCE_DIR}/include/ariadne-interval.hpp"
)

setup_project_library(
    TARGET ariadne-interval
    OBJECTS INTERVAL_SRC
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

if(BUILD_PYTHON_BINDINGS)
    add_subdirectory(python)
endif()

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    ariadne_install_dependency_bundle(
        TARGET interval
        DESTINATION include/ariadne-interval
    )
endif()
