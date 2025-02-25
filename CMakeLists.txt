cmake_minimum_required(VERSION 3.27)
project(PrimitiveChat)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

# Client executable
add_executable(
        PrimitiveChat_client

        # Client headers (headers can be omitted if you prefer, they're included via the source files)
        include/client_network.h
        include/client_outputs.h
        include/global.h
        include/interface.h

        # Client source files
        src/client/client.c
        src/client/client_network.c
        src/client/client_outputs.c
        src/client/interface.c
)

# Server executable
add_executable(
        PrimitiveChat_server

        # Server headers
        include/global.h
        include/server_network.h
        include/server_outputs.h

        # Server source files
        src/server/server.c
        src/server/server_network.c
        src/server/server_outputs.c
)

# Optionally, link CURL (if needed by client or server)
find_package(CURL REQUIRED)
if(CURL_FOUND)
    target_include_directories(PrimitiveChat_client PRIVATE ${CURL_INCLUDE_DIRS})
    target_link_libraries(PrimitiveChat_client PRIVATE ${CURL_LIBRARIES})
    # If the server needs CURL too, add similar commands:
    # target_include_directories(PrimitiveChat_server PRIVATE ${CURL_INCLUDE_DIRS})
    # target_link_libraries(PrimitiveChat_server PRIVATE ${CURL_LIBRARIES})
endif()
