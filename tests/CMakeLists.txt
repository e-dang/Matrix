find_package(Boost 1.71.0 COMPONENTS unit_test_framework REQUIRED)

if (Boost_FOUND)
    add_executable(test_matrix test_matrix.cpp)
    target_link_libraries(test_matrix ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    define_debug_definitions(test_matrix)
    add_test(NAME test_matrix COMMAND test_matrix)
endif()