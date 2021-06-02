cmake_minimum_required(VERSION 3.10)

project(RaftDateProtos VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD_REQUIRED True)

execute_process(COMMAND 
    bash -c 
    "protoc -I ${CMAKE_CURRENT_SOURCE_DIR}/src --grpc_out=${CMAKE_CURRENT_SOURCE_DIR} --plugin=protoc-gen-grpc=/raft-rpc/.local/bin/grpc_cpp_plugin ${CMAKE_CURRENT_SOURCE_DIR}/src/raft-date-server.proto")

execute_process(COMMAND 
    bash -c 
    "protoc -I ${CMAKE_CURRENT_SOURCE_DIR}/src --cpp_out=${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src/raft-date-server.proto")
