add_subdirectory(http)

set(NAME net)
set(SRCS
    src/response_handler.cpp
    src/acceptor.cpp
    src/tcp_connection.cpp
    src/tcp_server.cpp
)

add_library (${NAME} SHARED ${SRCS})
target_link_libraries(${NAME} 
    util 
    ${Boost_SYSTEM_LIBRARY} 
    ${GLOG_LIBRARY} 
    pthread)

install(TARGETS ${NAME}
   LIBRARY
   DESTINATION lib
   COMPONENT libraries)