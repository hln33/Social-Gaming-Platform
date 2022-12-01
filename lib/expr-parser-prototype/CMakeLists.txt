cmake_minimum_required(VERSION 3.12)

set(This express_parser)
set(CMAKE_CXX_STANDARD 17)

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "-std=c++17 ${CMAKE_CXX_FLAGS} -Wno-write-strings -Wno-unused-function")
    message(STATUS "detected gnucc")
    message(STATUS "using flags: ${CMAKE_CXX_FLAGS}")
endif()

find_package(BISON)
find_package(FLEX)

BISON_TARGET(bliniParser bliniGrammerSpec.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(bliniScanner bliniFlexSpec.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)

ADD_FLEX_BISON_DEPENDENCY(bliniScanner bliniParser)

add_executable(main
   driver.cpp
   ${BISON_bliniParser_OUTPUTS}
   ${FLEX_bliniScanner_OUTPUTS}
)

target_compile_features(main PRIVATE cxx_std_17)

target_link_libraries(main ${FLEX_LIBRARIES} Boost::variant)

target_include_directories(main
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>)


add_library(${This}
   ${BISON_bliniParser_OUTPUTS}
   ${FLEX_bliniScanner_OUTPUTS}
)

target_include_directories(${This}
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(${This} PRIVATE cxx_std_17)

set_target_properties(${This}
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)

target_link_libraries(${This}
                      ${FLEX_LIBRARIES} 
                      Boost::variant
)

install(TARGETS ${This}
  ARCHIVE DESTINATION lib
)
