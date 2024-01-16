cmake_minimum_required(VERSION 3.24)
project(EADS_LAB_3)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Wpedantic -pedantic-errors -Wno-unused-parameter -Wno-reorder)

add_executable(EADS_LAB_3 avl_tree_test.cpp avl_tree.h avl_tree_test.h)
configure_file(beagle_voyage.txt beagle_voyage.txt COPYONLY)
