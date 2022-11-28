add_executable(terminalClient
  src/terminalClient.cpp
  src/clientWindow.cpp
  src/clientHelper.cpp
)

add_custom_command(
        TARGET terminalClient POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/tools/terminalClient/test.json 
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.json )

set_target_properties(terminalClient
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

target_include_directories(terminalClient
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
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