set(This ruleHandler)

add_library(${This}
            src/ruleHandler.cpp
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
                    spdlog::spdlog
                    nlohmann_json::nlohmann_json
                    ruleProcessor
                    rules
              
)

install(TARGETS ${This}
  ARCHIVE DESTINATION lib
)
