cmake_minimum_required(VERSION 2.8)

project(angemon)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
find_package(Boost 1.68.0 REQUIRED COMPONENTS
             date_time filesystem iostreams )
include_directories(${Boost_INCLUDE_DIR})


add_compile_options(-std=c++11)
#set(CMAKE_CXX_STANDARD 11) //if cmake version >=3.1
#FILE(GLOB SRC_LIST "src/add-two-numbers/*.cpp")
add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
