aux_source_directory(. DIR_LIB_SRCS)

add_library(Sock  unix_socket.cpp unix_socket.h )
add_library(Ep  loop.h unix_loop.cpp unix_Loop.h)
add_library(Re  request.cpp request.h)


