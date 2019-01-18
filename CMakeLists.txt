cmake_minimum_required(VERSION 2.8)

project(angemon)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_compile_options(-std=c++11)
set( INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src" )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake" )
find_path( asio_INCLUDE asio.hpp HINTS   "/usr/local/include"  )

find_package( asio REQUIRED )
include_directories( ${INCLUDE_DIR} SYSTEM ${asio_INCLUDE}  )
file(GLOB DIR_SRCS "src/*")
add_executable(${PROJECT_NAME} ${DIR_SRCS})
