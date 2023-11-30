cmake_minimum_required(VERSION 3.27)
project(advent-of-code-2023 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_library(common common.cpp)

function(add_day day validationRegex)
    set(targetName "day-${day}")
    set(testName "${targetName}.test")
    add_executable(${targetName} day-${day}.cpp)
    target_link_libraries(${targetName} PRIVATE common)
    add_test(NAME ${testName} COMMAND ${targetName} WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
    set_tests_properties(${testName} PROPERTIES PASS_REGULAR_EXPRESSION "${validationRegex}")
endfunction()

add_day(01 " 55488\n.* 55614\n")
add_day(02 " 2265\n.* 64097\n")
