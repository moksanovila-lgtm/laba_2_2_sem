cmake_minimum_required(VERSION 3.10)
project(SequenceSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Только один раз!
add_subdirectory(library)

# Для GUI
add_subdirectory(app_gui)