add_library(handler
  src/handler.cpp
)

find_package(glog 0.4.0 REQUIRED)

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

target_link_libraries(handler 
                      glog::glog
                      nlohmann_json::nlohmann_json
                      GTest::gtest_main
)

install(TARGETS handler
  ARCHIVE DESTINATION lib
)

# comment out to disable unit testing
enable_testing()
add_subdirectory(test)