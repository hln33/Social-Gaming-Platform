set(This gameLogic)

add_library(${This}
        src/gameRules.cpp
        src/Player.cpp
        )

target_include_directories(${This}
        PUBLIC
        $<INSTALL_INTERFACE:includes>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
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
        )

install(TARGETS ${This}
        ARCHIVE DESTINATION lib
        )

# comment out to disable unit testing
#enable_testing()
#add_subdirectory(test)