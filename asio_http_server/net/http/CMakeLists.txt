set(NAME http)
set(SRCS
    src/http1_parser.cpp
    src/http_request.cpp
    src/http_response.cpp
    src/http_server.cpp
    ../../../externals/http-parser/http_parser.c
    )

add_library (${NAME} SHARED ${SRCS})
target_link_libraries(${NAME} ${Boost_SYSTEM_LIBRARY} ${GLOG_LIBRARY} pthread)

install(TARGETS ${NAME}
   LIBRARY
   DESTINATION lib
   COMPONENT libraries)
