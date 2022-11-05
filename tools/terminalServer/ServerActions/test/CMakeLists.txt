set(This test_terminalServer)

add_executable(${This} test.cpp)

target_link_libraries(${This}
    GTest::gtest_main
    ServerActions
    spdlog::spdlog
    nlohmann_json
)

add_test(
    NAME ${This}
    COMMAND ${This}
)