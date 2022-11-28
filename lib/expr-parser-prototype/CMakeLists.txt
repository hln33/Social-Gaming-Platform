cmake_minimum_required(VERSION 3.12)
project(blini-parser)

set(CMAKE_CXX_STANDARD 17)

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-write-strings -Wno-unused-function")
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
target_link_libraries(main ${FLEX_LIBRARIES})

target_include_directories(main
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>)
