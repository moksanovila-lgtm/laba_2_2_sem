cmake_minimum_required(VERSION 3.10)
project(SequenceLibrary)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==================== ѕќƒ Ћё„ј≈ћ «ј√ќЋќ¬ » ====================
# »нтерфейсна€ библиотека (все классы шаблонные, нет .cpp файлов)
add_library(SequenceLibrary INTERFACE)

# ƒиректори€ с заголовками
target_include_directories(SequenceLibrary INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ==================== ќѕ÷»ќЌјЋ№Ќќ: список заголовков дл€ IDE ====================
# Ёто не вли€ет на компил€цию, но помогает IDE видеть файлы
set(HEADERS
    include/ICollection.hpp
    include/IEnumerator.hpp
    include/Sequence.hpp
    include/Sequence.tpp
    include/DynamicArray.hpp
    include/DynamicArray.tpp
    include/LinkedList.hpp
    include/LinkedList.tpp
    include/ArraySequence.hpp
    include/ArraySequence.tpp
    include/ListSequence.hpp
    include/ListSequence.tpp
    include/ImmutableArraySequence.hpp
    include/ImmutableListSequence.hpp
    include/Bit.hpp
    include/BitSequence.hpp
    include/BitSequence.tpp
    include/exceptions.hpp
    include/Utils.hpp
)

# ƒл€ IDE: показать заголовки в проекте
source_group("Header Files" FILES ${HEADERS})