cmake_minimum_required(VERSION 3.17)
project(client)

set(CMAKE_CXX_STANDARD 14)

#set(Boost_USE_STATIC_LIBS_OFF)
#set(Boost_USE_MULTITHREADED_ON)
#set(Boost_USE_STATIC_RUNTIME_OFF)
#find_package(Boost COMPONENTS system)
#
#if(Boost_FOUND)
#    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(client include/connectionHandler.h src/connectionHandler.cpp src/echoClient.cpp include/Task.h src/Task.cpp)
#    target_link_libraries(client ${Boost_LIBRARIES})
#endif()

