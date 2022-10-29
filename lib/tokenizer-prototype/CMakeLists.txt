cmake_minimum_required(VERSION 3.12)
project(blini-flexer)

find_package(FLEX)

FLEX_TARGET(bliniFlexer bliniFlexSpec.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)

add_executable(main
   driver.cpp
   ${FLEX_bliniFlexer_OUTPUTS}
)
target_link_libraries(main ${FLEX_LIBRARIES})

target_include_directories(sortIntegers
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>)
