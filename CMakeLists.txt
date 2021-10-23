# Copyright © Alexander Kaluzhnyy
cmake_minimum_required(VERSION 3.21)

project(word_cup)

include(common.cmake)

set(word_cup_c_sources
    c/word_cup.c
    c/read_word.c
    c/word_holder.c
    c/get_best_words.c
)

add_executable(word_cup_c
    ${word_cup_c_sources}
)

add_executable(word_cup_cpp
    cpp/word_cup.cpp
)

#TESTING

add_executable(test_word_cup_c
    ${word_cup_c_sources}
    c/test/test_word_cup_c.cpp
)
target_link_libraries(test_word_cup_c gtest)

add_test(
    NAME test_word_cup_c
    COMMAND test_word_cup_c
)

add_executable(test_word_cup_cpp
    cpp/word_cup.cpp
    cpp/test/test_word_cup_cpp.cpp
)
target_link_libraries(test_word_cup_cpp gtest)

add_test(
    NAME test_word_cup_cpp
    COMMAND test_word_cup_cpp
)
