set(SRCS
        main.cpp
)

add_executable(http_server_test ${SRCS})

target_link_libraries(http_server_test
        http
        net
        ${Boost_SYSTEM_LIBRARY} ${GLOG_LIBRARY} pthread
)

install(TARGETS http_server_test
        RUNTIME
        DESTINATION bin)
