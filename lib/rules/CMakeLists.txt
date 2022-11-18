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

