set(This gameLogic)

add_library(${This}
        src/Player.cpp
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
        Responses
        )

install(TARGETS ${This}
        ARCHIVE DESTINATION lib
        )
        