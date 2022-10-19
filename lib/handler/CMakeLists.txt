set(This handler)

add_library(${This}
  src/handler.cpp
)

find_package(glog 0.4.0 REQUIRED)

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
                      glog::glog
                      nlohmann_json::nlohmann_json
)

install(TARGETS ${This}
  ARCHIVE DESTINATION lib
)

# comment out to disable unit testing
enable_testing()
add_subdirectory(test)