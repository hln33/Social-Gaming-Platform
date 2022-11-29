set(This InputActionHandler)

add_library(${This}
          ServerActionHandler.cpp
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
                    networking
                    nlohmann_json::nlohmann_json
                    spdlog::spdlog
                    handler
)

install(TARGETS ${This}
  ARCHIVE DESTINATION lib
)