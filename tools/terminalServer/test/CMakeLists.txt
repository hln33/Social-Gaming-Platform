set(This test_terminalServer)
set(lib terminalServerTest)

add_library(${lib}
  ../src/terminalServer.cpp
)

target_include_directories(${lib}
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

target_link_libraries(${lib}
  networking
  handler
  nlohmann_json
  spdlog::spdlog
  ${CMAKE_THREAD_LIBS_INIT}
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