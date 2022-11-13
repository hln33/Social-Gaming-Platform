set(This gameLogic)

add_library(${This}
        src/gameRules.cpp
        src/Player.cpp
        src/GameObject.cpp
        src/UniqueId.cpp
        src/Room.cpp
        src/RoomConfig.cpp
        #src/PlayerStorage.cpp
        #src/unique_id_builder.cpp
        #these are broken
        #src/room.cpp
        #src/player_storage.cpp
        )

target_include_directories(${This}
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

set_target_properties(${This}
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
        )

target_link_libraries(${This}
        nlohmann_json::nlohmann_json
        GTest::gtest_main
        handler
        )

install(TARGETS ${This}
        ARCHIVE DESTINATION lib
        )
        
# comment out to disable unit testing
#enable_testing()
#add_subdirectory(test)


add_executable(fixPolicyBugHaHa
    drivers/fix-policy-bug.cpp
)

set_target_properties(fixPolicyBugHaHa
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

target_include_directories(fixPolicyBugHaHa
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(fixPolicyBugHaHa
  ${This}
)