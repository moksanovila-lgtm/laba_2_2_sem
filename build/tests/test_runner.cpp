#include <gtest/gtest.h>

// Функция main уже есть в gtest_main
// Этот файл может быть пустым или содержать глобальные настройки

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}