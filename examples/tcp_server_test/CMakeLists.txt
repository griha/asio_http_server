set(SRCS
    main.cpp   
)
   
add_executable(tcp_server_test ${SRCS})

target_link_libraries(tcp_server_test
                      net
                      ${Boost_SYSTEM_LIBRARY} ${GLOG_LIBRARY} pthread
                      )

install(TARGETS tcp_server_test
   RUNTIME
   DESTINATION bin)
