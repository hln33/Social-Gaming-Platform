enable_testing()

add_executable(runAllTests
    messageProcessorTests.cpp
    handlerTests.cpp
    controllerTests.cpp
    rulesUnitTests.cpp
)

target_link_libraries(runAllTests
    gmock gtest gtest_main
    spdlog::spdlog
    handler
    rules
)

set_target_properties(runAllTests
    PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test
)

add_test(NAME AllTests COMMAND runAllTests)