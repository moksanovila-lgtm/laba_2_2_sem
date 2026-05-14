cmake_minimum_required(VERSION 3.14)
project(SequenceTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(TEST_SOURCES
    test_dynamic_array.cpp
    test_linked_list.cpp
    test_array_sequence.cpp
    test_list_sequence.cpp
    test_bit_sequence.cpp
    test_immutable.cpp
    test_exceptions.cpp
)

add_executable(sequence_tests ${TEST_SOURCES})

target_link_libraries(sequence_tests
    SequenceLibrary
    GTest::gtest_main
)

target_include_directories(sequence_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../library/include
)

include(GoogleTest)
gtest_discover_tests(sequence_tests)