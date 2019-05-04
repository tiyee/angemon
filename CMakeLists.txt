cmake_minimum_required(VERSION 2.8)
project(angemon)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
#SET(CMAKE_C_COMPILER "/usr/bin/clang")
#SET (CMAKE_CXX_COMPILER  "/usr/bin/clang++")
#set(CMAKE_CXX_FLAGS " -stdlib=libc++")






SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
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

aux_source_directory(src DIR_SRCS)
# 添加头文件路径
include_directories(./ ${PROJECT_SOURCE_DIR}/src/angemon)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/angemon)
add_executable(angemon ${DIR_SRCS})
target_link_libraries(angemon  Sock Ep Re)

