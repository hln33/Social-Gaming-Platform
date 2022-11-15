add_executable(runRoomTests
  RoomUnitTests.cpp
)

target_link_libraries(runRoomTests
  gmock gtest gtest_main
  gameLogic
)

add_test(NAME AllTests COMMAND runRoomTests)
