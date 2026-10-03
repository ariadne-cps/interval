include_directories(${Python3_INCLUDE_DIRS})

get_target_property(PYTHON_COMMON_SOURCE_DIR pyariadne-python-common SOURCE_DIR)
get_target_property(NUMERIC_SOURCE_DIR numeric SOURCE_DIR)
get_target_property(FOUNDATION_SOURCE_DIR foundation SOURCE_DIR)

add_library(pyariadne-interval-submodule-obj OBJECT
    interval_submodule.cpp
)
target_include_directories(pyariadne-interval-submodule-obj PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${PYTHON_COMMON_SOURCE_DIR}/include"
    "${NUMERIC_SOURCE_DIR}/python/bindings"
    "${FOUNDATION_SOURCE_DIR}/python/bindings"
)
target_link_libraries(pyariadne-interval-submodule-obj PRIVATE
    pyariadne-python-common
    pybind11::module
    interval
)
set_target_properties(pyariadne-interval-submodule-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)
if(MSVC)
    target_compile_options(pyariadne-interval-submodule-obj PRIVATE /bigobj)
endif()

add_library(pyariadne-interval INTERFACE)
target_sources(pyariadne-interval INTERFACE
    $<TARGET_OBJECTS:pyariadne-interval-submodule-obj>
)
target_include_directories(pyariadne-interval INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(pyariadne-interval INTERFACE
    pyariadne-numeric
)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    add_library(pyariadne-interval-module-obj OBJECT
        interval_module.cpp
    )
    target_include_directories(pyariadne-interval-module-obj PRIVATE
        "${PYTHON_COMMON_SOURCE_DIR}/include"
    )
    target_link_libraries(pyariadne-interval-module-obj PRIVATE
        pyariadne-python-common
        pybind11::module
    )
    set_target_properties(pyariadne-interval-module-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
