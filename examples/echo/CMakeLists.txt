set(SRCS
    main.cpp   
)
   
add_executable(echo_server ${SRCS})

install(TARGETS echo_server
   RUNTIME
   DESTINATION bin)
