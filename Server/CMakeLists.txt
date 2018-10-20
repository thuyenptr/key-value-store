cmake_minimum_required(VERSION 3.12)
project(Server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
ADD_SUBDIRECTORY(protoc)
#add_executable(Server main.cpp include/KeyValueStore.h include/BTreeStorage.h include/BTreeNode.h include/BTree.h include/DataStore.h include/server.h include/utils.h include/Global.h include/CondVar.h include/Mutex.h include/Task.h include/ThreadPool.h src/BTreeStorage.cpp src/BTreeNode.cpp src/BTree.cpp src/server.cpp src/utils.cpp src/Global.cpp src/CondVar.cpp src/Mutex.cpp src/ThreadPool.cpp src/Task.cpp)
add_executable(Server main.cpp include/KeyValueStore.h include/BTreeStorage.h include/BTreeNode.h include/BTree.h include/DataStore.h include/server.h include/utils.h include/Global.h include/CondVar.h include/Mutex.h include/Task.h include/ThreadPool.h test/BTreeStorage.cpp test/BTreeNode.cpp test/BTree.cpp test/server.cpp test/utils.cpp test/Global.cpp test/CondVar.cpp test/Mutex.cpp test/ThreadPool.cpp test/Task.cpp)

TARGET_LINK_LIBRARIES(Server proto)
