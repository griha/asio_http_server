set(NAME http)
set(SRCS
#    src/http_parser.cpp
    src/http1_parser.cpp
    src/http_request.cpp
    src/http_response.cpp
    )

add_library (${NAME} SHARED ${SRCS})
target_link_libraries(${NAME} ${Boost_SYSTEM_LIBRARY} ${GLOG_LIBRARY} pthread)

install(TARGETS ${NAME}
   LIBRARY
   DESTINATION lib
   COMPONENT libraries)
