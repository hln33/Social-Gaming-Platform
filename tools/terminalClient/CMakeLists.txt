add_executable(terminalClient
  terminalClient.cpp
  clientWindow.cpp
)

set_target_properties(terminalClient
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(terminalClient
  networking
  nlohmann_json
  ${CURSES_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS terminalClient
  RUNTIME DESTINATION bin
)