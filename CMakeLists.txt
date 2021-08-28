cmake_minimum_required(VERSION 3.0)
project(angemon VERSION 1.0.0)
cmake_policy(SET CMP0074 NEW)
set(CMAKE_CXX_STANDARD 11)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O0 -Wall -g -ggdb") #for lldb or gdb


set(DEP_ROOT_DIR ${CMAKE_BINARY_DIR}/thirdpart-${CMAKE_SYSTEM_NAME})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(BUILD_DEPS "yes" CACHE STRING "")


if(NOT EXISTS ${DEP_ROOT_DIR})
    file(MAKE_DIRECTORY ${DEP_ROOT_DIR})
endif()

SET(CMAKE_FIND_LIBRARY_SUFFIXES .a)
set(ENV{CMAKE_FIND_LIBRARY_SUFFIXES} ".a")
include(cmake/Jemalloc.cmake)
include(cmake/GoogleTest.cmake)

include_directories(${DEP_ROOT_DIR}/${JEMALLOC_NAME}/include)
include_directories(${DEP_ROOT_DIR}/${GTEST_NAME}/googletest/include)
link_directories(${DEP_ROOT_DIR}/${JEMALLOC_NAME}/lib)
link_directories(${DEP_ROOT_DIR}/${GTEST_NAME}/googletest/build)
set(JEMALLOC_ROOT_DIR "${DEP_ROOT_DIR}/${JEMALLOC_NAME}")
FIND_PACKAGE(Jemalloc REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

FILE(GLOB SRC_FILES src/*.cpp )
FILE(GLOB TEST_FILES tests/*.cpp)
FILE(GLOB SRC_INCLUDE_FILES src/*.h )

add_definitions("-D__JEMALLOC__")
include_directories(src)


add_executable(${PROJECT_NAME} ${SRC_FILES} main.cpp)
add_executable(${PROJECT_NAME}-test ${SRC_FILES} ${TEST_FILES} )

target_compile_definitions(
        ${PROJECT_NAME} PRIVATE
        VERSION="1.0.0"
)
target_compile_definitions(
        ${PROJECT_NAME}-test PRIVATE
        ROOT_DIR="${CMAKE_SOURCE_DIR}/"
        VERSION="1.0.0"
)


set(CORE_LIBS  ${JEMALLOC_LIBRARIES}
        pthread dl )

target_link_libraries(${PROJECT_NAME} ${CORE_LIBS})

target_link_libraries(${PROJECT_NAME}-test ${CORE_LIBS} gtest gtest_main)










