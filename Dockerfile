FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt update

RUN apt install -y cmake build-essential autoconf libtool pkg-config git

RUN mkdir -p /raft-rpc/.local

ENV PATH "/raft-rpc/.local/bin:$PATH"

WORKDIR /raft-rpc/.local

RUN git clone --recurse-submodules -b v1.37.1 https://github.com/grpc/grpc

WORKDIR /raft-rpc/.local/grpc

RUN mkdir -p /raft-rpc/.local/grpc/cmake/build

WORKDIR /raft-rpc/.local/grpc/cmake/build

RUN cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/raft-rpc/.local ../..

RUN make -j 1

RUN make install

RUN mkdir -p /raft-rpc/.local/grpc/third_party/abseil-cpp/cmake/build

WORKDIR /raft-rpc/.local/grpc/third_party/abseil-cpp/cmake/build

RUN cmake -DCMAKE_INSTALL_PREFIX=/raft-rpc/.local -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE ../..

RUN make -j 1

RUN make install

WORKDIR /raft-rpc

COPY . .

WORKDIR /raft-rpc/protos

RUN cmake .

RUN make -j 1

WORKDIR /raft-rpc

RUN cmake .

RUN make -j 1