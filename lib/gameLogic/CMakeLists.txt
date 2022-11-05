add_library(Core
  src/UniqueId.cpp
  src/PlayerStorage.cpp
  src/Room.cpp
)

target_include_directories(Core
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(Core
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)


install(TARGETS Core
  ARCHIVE DESTINATION lib
)
