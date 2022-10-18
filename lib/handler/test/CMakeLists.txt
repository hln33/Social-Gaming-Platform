set(This exampleTests)

add_executable(${This} exampleTests.cpp)

target_link_libraries(${This}
    GTest::gtest_main
)

add_test(
    NAME ${This}
    COMMAND ${This}
)