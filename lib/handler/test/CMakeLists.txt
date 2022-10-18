add_executable(exampleTests exampleTests.cpp)

target_link_libraries(exampleTests
    GTest::gtest_main
)

add_test(
    NAME exampleTests
    COMMAND exampleTests
)