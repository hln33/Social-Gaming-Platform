add_subdirectory(Actions)

add_executable(terminalServer
terminalServer.cpp
)

add_custom_command(
        TARGET terminalServer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/tools/terminalServer/webchat.html 
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/webchat.html )
                
set_target_properties(terminalServer
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)
 
find_package(Threads REQUIRED)

target_link_libraries(terminalServer
  networking
  ServerActions
  nlohmann_json
  spdlog::spdlog
  ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS terminalServer
  RUNTIME DESTINATION bin
)

install(FILES ${CMAKE_SOURCE_DIR}/tools/terminalServer/webchat.html RUNTIME DESTINATION bin)

# comment out to disable unit testing
#enable_testing()
#add_subdirectory(test)