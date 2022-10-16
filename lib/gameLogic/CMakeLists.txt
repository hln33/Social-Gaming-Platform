add_library(gameLogic
  src/gameLogic.cpp
)

target_include_directories(gameLogic
  PUBLIC
    $<INSTALL_INTERFACE:includes>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(gameLogic
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)

target_link_libraries(gameLogic) 

install(TARGETS gameLogic
  ARCHIVE DESTINATION lib
)

