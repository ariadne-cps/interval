include(CTest)

add_executable(test_interval_testing test_interval_testing.cpp)
target_link_libraries(test_interval_testing PRIVATE ariadne-interval interval ariadne-testing)
add_test(NAME test_interval_testing COMMAND $<TARGET_FILE:test_interval_testing> --concise)
if(WIN32)
    set_tests_properties(test_interval_testing PROPERTIES
        ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:$<TARGET_FILE_DIR:ariadne-interval>")
endif()

add_executable(test_interval test_interval.cpp)
target_link_libraries(test_interval PRIVATE ariadne-interval interval ariadne-testing)
add_test(NAME test_interval COMMAND $<TARGET_FILE:test_interval> --concise)
if(WIN32)
    set_tests_properties(test_interval PROPERTIES
        ENVIRONMENT_MODIFICATION
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:ariadne-interval>"
    )
endif()

add_custom_target(tests)
add_dependencies(tests test_interval test_interval_testing)
