cmake_minimum_required(VERSION 3.0.0)

project(Matrix VERSION 0.1.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include(CTest)
enable_testing()

cmake_policy(SET CMP0064 NEW) # allow TEST operator in 'if' statements
function(define_debug_definitions TARGET)
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR TEST TARGET)
        target_compile_definitions(${TARGET} PUBLIC DEBUG)
    endif()
endfunction()

include_directories(include)
add_subdirectory(tests)