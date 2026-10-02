include(CTest)

add_executable(test_interval test_interval.cpp)
target_link_libraries(test_interval ariadne-interval interval)
add_test(NAME test_interval COMMAND $<TARGET_FILE:test_interval>)
if(WIN32)
    set_tests_properties(test_interval PROPERTIES
        ENVIRONMENT_MODIFICATION
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:ariadne-interval>"
    )
endif()

add_custom_target(tests)
add_dependencies(tests test_interval)
