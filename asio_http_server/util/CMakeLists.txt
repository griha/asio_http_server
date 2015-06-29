set(NAME util)
set(SRCS
    src/thread_pool.cpp
    )

add_library (${NAME} SHARED ${SRCS})
target_link_libraries(${NAME} ${Boost_SYSTEM_LIBRARY} ${GLOG_LIBRARY} pthread)

install(TARGETS ${NAME}
   LIBRARY
   DESTINATION lib
   COMPONENT libraries)