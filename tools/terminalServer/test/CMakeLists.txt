set(This Test)

add_library(terminalServerTest
  ../src/terminalServer.cpp
)

add_executable(${This} test.cpp)

target_link_libraries(${This}
    GTest::gtest_main
    terminalServerTest
)

add_test(
    NAME ${This}
    COMMAND ${This}
)