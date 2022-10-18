set(This exampleTests)

add_executable(${This} exampleTests.cpp)

target_link_libraries(${This}
    GTest::gtest_main
    handler
)

add_test(
    NAME ${This}
    COMMAND ${This}
)