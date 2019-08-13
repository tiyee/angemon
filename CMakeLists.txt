cmake_minimum_required(VERSION 2.8)
project(angemon)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
#set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -stdlib=libc++")






SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -m64 -O2")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -m64 -O2")
if (CMAKE_HOST_APPLE)
    add_definitions(-DOS_MACOSX)
elseif (CMAKE_HOST_UNIX)
    add_definitions(-DOS_LINUX)
else (CMAKE_HOST_APPLE)
    message(FATAL_ERROR "Platform not supported")
endif (CMAKE_HOST_APPLE)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

MESSAGE("++++++++++++++Start Build+++++++++++++++++")

link_directories(${PROJECT_SOURCE_DIR}/lib)

file(GLOB SRC_DIR "src/*.cpp")
add_executable(angemon src/main.cpp ${SRC_DIR} )


