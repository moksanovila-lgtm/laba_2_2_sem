cmake_minimum_required(VERSION 3.14) # Рекомендуемая версия для FetchContent
project(SequenceSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Подключаем основные подпроекты ---
add_subdirectory(library)
add_subdirectory(app_gui) # или app_console

# --- НАСТРОЙКА ТЕСТОВ ---
# Включаем поддержку тестов в CMake
enable_testing()

# Подключаем Google Test с помощью FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  # Используем актуальную версию. Вы можете указать конкретный тег, например, v1.15.0
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.0.zip
)
# Для Windows: предотвращаем переопределение настроек компилятора/линкера
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Добавляем директорию с тестами
add_subdirectory(tests)