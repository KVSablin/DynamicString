cmake_minimum_required(VERSION 3.15)
project(dynamic_string_lib VERSION 0.0.1)

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME}
    INTERFACE
        ${PROJECT_SOURCE_DIR}/BasicString.h
        ${PROJECT_SOURCE_DIR}/Allocator.h
        ${PROJECT_SOURCE_DIR}/CharTraits.h
        ${PROJECT_SOURCE_DIR}/CString.h
        ${PROJECT_SOURCE_DIR}/DynamicString
)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11)
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR})