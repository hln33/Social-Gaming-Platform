enable_testing()

add_executable(runExpressTests
    ExpressionVisitorTests.cpp
)

target_link_libraries(runExpressTests
    gmock gtest gtest_main
    spdlog::spdlog
    express_parser
)

set_target_properties(runExpressTests
    PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test
)

add_test(NAME ExpressTests COMMAND runExpressTests)