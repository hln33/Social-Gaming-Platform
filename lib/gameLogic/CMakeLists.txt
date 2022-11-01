
add_library(UniqueId
  src/UniqueId.cpp
)

target_include_directories(UniqueId
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(UniqueId
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)


install(TARGETS UniqueId
  ARCHIVE DESTINATION lib
)


add_library(PlayerStorage
  src/PlayerStorage.cpp
)

target_include_directories(PlayerStorage
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(PlayerStorage
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)


install(TARGETS PlayerStorage
  ARCHIVE DESTINATION lib
)


add_library(Room
  src/Room.cpp
)

target_include_directories(Room
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(Room
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)


install(TARGETS Room
  ARCHIVE DESTINATION lib
)

# for unit testing
# add_subdirectory(tests)
