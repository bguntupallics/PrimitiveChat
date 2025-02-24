cmake_minimum_required(VERSION 3.27)
project(PrimitiveChat)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(
        PrimitiveChat

        include/client_network.h
        include/client_outputs.h
        include/global.h
        include/interface.h
        include/parser.h
        include/server_network.h
        include/server_outputs.h

        src/client/client.c
        src/client/client_network.c
        src/client/client_outputs.c
        src/client/interface.c

        src/nlp/cs.cpp
        src/nlp/test.cpp

        src/server/parser.c
        src/server/server.c
        src/server/server_network.c
        src/server/server_outputs.c
)

find_package(CURL REQUIRED)
if(CURL_FOUND)
    target_include_directories(PrimitiveChat PRIVATE ${CURL_INCLUDE_DIRS})
    target_link_libraries(PrimitiveChat PRIVATE ${CURL_LIBRARIES})
endif()
