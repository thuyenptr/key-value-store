cmake_minimum_required(VERSION 3.12)
project(Server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
ADD_SUBDIRECTORY(protoc)
add_executable(Server main.cpp include/KeyValueStore.h src/BTreeStorage.cpp include/BTreeStorage.h src/BTreeNode.cpp include/BTreeNode.h src/BTree.cpp include/BTree.h include/DataStore.h src/server.cpp include/server.h src/utils.cpp include/utils.h src/Global.cpp include/Global.h src/CondVar.cpp include/CondVar.h src/Mutex.cpp include/Mutex.h src/Task.cpp include/Task.h src/ThreadPool.cpp include/ThreadPool.h)
TARGET_LINK_LIBRARIES(Server proto)