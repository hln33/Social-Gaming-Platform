add_executable(terminalServer
terminalServer.cpp
)

set_target_properties(terminalServer
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

find_package(Threads REQUIRED)

target_link_libraries(terminalServer
  networking
  handler
  nlohmann_json
  spdlog::spdlog
  ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS terminalServer
  RUNTIME DESTINATION bin
)