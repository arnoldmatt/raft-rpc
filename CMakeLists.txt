cmake_minimum_required(VERSION 3.10)

project(RaftDateServer VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD_REQUIRED True)

# Threads required for building grpc in next step
find_package(Threads REQUIRED)

# Build grpc into this project directly
add_subdirectory(/raft-rpc/.local/grpc 
    ${CMAKE_CURRENT_BINARY_DIR}/grpc 
    EXCLUDE_FROM_ALL)

# After using add_subdirectory, we can now use the grpc targets directly from  #
# this build.                                                                  #
set(_PROTOBUF_LIBPROTOBUF libprotobuf)                                         #
set(_REFLECTION grpc++_reflection)                                             #
if(CMAKE_CROSSCOMPILING)                                                       #
    find_program(_PROTOBUF_PROTOC protoc)                                      #
else()                                                                         #
    set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)                      #
endif()                                                                        #
set(_GRPC_GRPCPP grpc++)                                                       #
if(CMAKE_CROSSCOMPILING)                                                       #
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)                  #
else()                                                                         #
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)            #
endif()                                                                        #

# Include generated *.pb.h files
set(hw_proto_srcs 
    "${CMAKE_CURRENT_SOURCE_DIR}/protos/raft-date-server.pb.cc")
set(hw_proto_hdrs 
    "${CMAKE_CURRENT_SOURCE_DIR}/protos/raft-date-server.pb.h")
set(hw_grpc_srcs 
    "${CMAKE_CURRENT_SOURCE_DIR}/protos/raft-date-server.grpc.pb.cc")
set(hw_grpc_hdrs 
    "${CMAKE_CURRENT_SOURCE_DIR}/protos/raft-date-server.grpc.pb.h")

# Standard includes
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/protos")

# Create library for the generated grpc files
add_library(hw_grpc_proto
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs})
target_link_libraries(hw_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# Server Main compilation
add_executable(RaftDateServer 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/raft-date-server.cc)

# Server Main linker
target_link_libraries(RaftDateServer
    hw_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})

# Client Main compilation
add_executable(RaftDateClient
    ${CMAKE_CURRENT_SOURCE_DIR}/src/raft-date-client.cc)

# Client Main linker
target_link_libraries(RaftDateClient
    hw_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})