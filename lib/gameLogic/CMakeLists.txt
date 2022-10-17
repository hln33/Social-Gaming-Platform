add_library(gameLogic
  src/gameLogic.cpp
  src/room.cpp
  src/player_storage.cpp
  src/Player.cpp
  src/unique_id.cpp
)

target_include_directories(gameLogic
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(gameLogic) 

set_target_properties(gameLogic
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)

install(TARGETS gameLogic
  ARCHIVE DESTINATION lib
)

