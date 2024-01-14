cmake_minimum_required(VERSION 3.25)
project(EADS_LAB_3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(EADS_LAB_3
        avl_tree.h
        avl_tree_test.cpp
        avl_tree_test.h)
