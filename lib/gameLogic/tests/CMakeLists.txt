enable_testing()

add_executable(runRoomTests
  RoomUnitTests.cpp
)

target_link_libraries(runRoomTests
  gmock gtest gtest_main
  Room PlayerStorage UniqueId
)

add_test(NAME AllTests COMMAND runRoomTests)
