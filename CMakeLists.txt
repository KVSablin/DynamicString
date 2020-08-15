cmake_minimum_required(VERSION 3.15)
project(DynamicString LANGUAGES CXX VERSION 0.0.1)

include(GNUInstallDirs)

add_subdirectory(include)
add_subdirectory(example)

include(cmake/googletest.cmake)
build_googletest()

enable_testing()
add_subdirectory(test)