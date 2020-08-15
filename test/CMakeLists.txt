add_executable(unit_test test.cpp)

target_link_libraries(
        unit_test
        dynamic_string_lib
        gtest_main
)

target_include_directories(unit_test PRIVATE ${PROJECT_SOURCE_DIR})

add_test(
    NAME utest
    COMMAND unit_test
)