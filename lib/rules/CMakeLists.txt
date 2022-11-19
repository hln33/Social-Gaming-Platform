project(Blini)

cmake_minimum_required(VERSION 3.19)

add_executable(rules driver.cpp)

target_include_directories(rules
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

set_target_properties(rules
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
        )

# set(This rules)

# add_library(${This}
#         src/Rules.cpp
# )

# target_include_directories(${This}
#         PUBLIC
#         $<INSTALL_INTERFACE:include>
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#         PRIVATE
#         ${CMAKE_CURRENT_SOURCE_DIR}/src
#         )

# set_target_properties(${This}
#         PROPERTIES
#         LINKER_LANGUAGE CXX
#         CXX_STANDARD 17
# )       

# target_link_libraries(${This}
#         nlohmann_json::nlohmann_json
#         GTest::gtest_main
#         handler
#         )

# install(TARGETS ${This}
# ARCHIVE DESTINATION lib
# )