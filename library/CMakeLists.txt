cmake_minimum_required(VERSION 3.14)
project(SequenceLibrary)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")
add_library(SequenceLibrary STATIC ${SOURCES})

target_include_directories(SequenceLibrary PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)