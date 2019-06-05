cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(gatesdk-cpp-server)
set(PROTOBUF_VERSION v3.7.1)
set(LibSource  src/oldstdutils.h src/servergateevents.h src/gateevents.h src/WaitEvent.cpp src/WaitEvent.h src/SocketUtil.cpp src/SocketUtil.h src/gateconnection.cpp src/gateconnection.h src/CLock.cpp src/CLock.h
 src/ByteBuf.cpp src/ByteBuf.h src/csgateconnection.cpp src/csgateconnection.h src/gateconnection.h src/gateconnection.cpp
     src/message/ClientSessionMessage.pb.cc src/message/ClientSessionMessage.pb.h 
     src/message/ServerSessionMessage.pb.cc src/message/ServerSessionMessage.pb.h )

include_directories(./src ./src/message ./dependency/protobuf/${PROTOBUF_VERSION}/include)
link_directories(${CMAKE_SOURCE_DIR}/dependency/protobuf/${PROTOBUF_VERSION})
add_executable(gatesdk-server-test main.cpp ${LibSource})
target_link_libraries(gatesdk-server-test debug libprotobuf-lited.lib optimized libprotobuf-lite.lib)

