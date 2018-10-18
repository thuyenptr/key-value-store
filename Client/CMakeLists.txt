cmake_minimum_required(VERSION 3.12)
project(Client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lprotoc -pthread -g -lrt")

ADD_SUBDIRECTORY(protoc)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
add_executable(Client main.cpp src/client.cpp include/client.h include/config.h)
TARGET_LINK_LIBRARIES(Client proto)