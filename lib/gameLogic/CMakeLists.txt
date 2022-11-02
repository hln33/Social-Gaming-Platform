set(This gameLogic)

add_library(${This}
  # src/GameObject.cpp
  # src/gameRules.cpp
  # src/Player.cpp
  # src/symbol_table.cpp
  # src/unique_id_builder.cpp
  src/UniqueId.cpp
  src/PlayerStorage.cpp
  src/Room.cpp
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


install(TARGETS ${This}
  ARCHIVE DESTINATION lib
)
