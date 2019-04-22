cmake_minimum_required(VERSION 2.8)

project(angemon)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_compile_options(-std=c++11)



file(GLOB DIR_SRCS "src/angemon/*")
file(GLOB POOL "src/angemon/pool/*")
add_executable(${PROJECT_NAME} "main.cpp" ${DIR_SRCS} ${POOL})
