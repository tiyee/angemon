aux_source_directory(. DIR_LIB_SRCS)
add_library(Accptor  acceptor.cpp)
if (CMAKE_HOST_APPLE)
add_library(Sock socket.cpp socket.h)
add_library(Loop base_loop.h unix_loop.cpp unix_loop.h)


elseif (CMAKE_HOST_UNIX)
    add_library(Sock socket.cpp socket.h)
    add_library(Loop base_loop.h linux_loop.cpp linux_loop.h)

else (CMAKE_HOST_APPLE)
    message(FATAL_ERROR "Platform not supported")
endif (CMAKE_HOST_APPLE)
