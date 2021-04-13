# source cmake

set(FLOTON_SOURCE_FILES
    SocketFormat.c
    TcpServer.cpp
    ObjectTypes.cpp
    Rows.cpp
)

add_library(${FLOTON_STATIC_LIB_NAME} STATIC "${FLOTON_SOURCE_FILES}")

add_subdirectory(main)