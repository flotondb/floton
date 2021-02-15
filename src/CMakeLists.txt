# source cmake

set(FLOTON_SOURCE_FILES
    SocketFormat.c
    TcpServer.cpp
)

add_library(flotoncore STATIC "${FLOTON_SOURCE_FILES}")

add_subdirectory(main)