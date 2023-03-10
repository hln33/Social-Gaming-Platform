set(This handler)

add_library(${This}
  src/handler.cpp
  src/controller.cpp
  src/handlerHelper.cpp
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
                      gameLogic
                      networking
                      spdlog::spdlog
                      ruleProcessor
)

install(TARGETS ${This}
  ARCHIVE DESTINATION lib
)
