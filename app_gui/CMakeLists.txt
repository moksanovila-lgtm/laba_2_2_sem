cmake_minimum_required(VERSION 3.16)
project(SequenceGUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/SequenceController.cpp
    src/SequenceWidget.cpp
)

set(HEADERS
    include/MainWindow.hpp
    include/SequenceController.hpp
    include/SequenceWidget.hpp
)

add_executable(SequenceGUI ${SOURCES} ${HEADERS})

target_include_directories(SequenceGUI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../library/include
)

target_link_libraries(SequenceGUI
    SequenceLibrary
    Qt6::Widgets
)