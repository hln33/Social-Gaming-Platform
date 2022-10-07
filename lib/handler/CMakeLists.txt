add_library(handler
  src/handler.cpp
)

find_package(glog 0.6.0 REQUIRED)

target_include_directories(handler
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(handler
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)

target_link_libraries(handler glog::glog)

install(TARGETS handler
  ARCHIVE DESTINATION lib
)


# creates a handler-test executable that I (Harry) used to test the handler code
# comment out the below block if you do not want to make the executable when you build
add_executable(handler-test src/main.cpp)
target_include_directories(handler-test
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
set_target_properties(handler-test
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)
target_link_libraries(handler-test handler)