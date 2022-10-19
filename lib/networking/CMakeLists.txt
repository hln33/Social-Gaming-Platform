add_library(networking
  src/Server.cpp
  src/Client.cpp
  src/NetworkingHelper.cpp
  include/NetworkMessage.h
)

set(Boost_Libraries
    Boost::system 
    Boost::asio 
    Boost::random 
    Boost::beast
)

target_include_directories(networking
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries( networking
                      ${Boost_LIBRARIES}
)

set_target_properties(networking
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)

install(TARGETS networking
  ARCHIVE DESTINATION lib
)