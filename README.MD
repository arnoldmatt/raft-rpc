raft-rpc - An RPC client/server application for Raft
====================================================

raft-rpc is a client/server application that utilizes <a href="https://grpc.io/">gRPC</a> (remote procedure call (RPC) framework) to simplify Raft client and server application communications.

# To start using raft-rpc

To start using raft-rpc, you will need to install the following pre-requisites on your build machine:

 * **Docker - v20.10.6**

Then, perform the following steps to 1) build the raft-rpc image, 2) start the rpc server, 3) run the client

# To build the raft-rpc image

> gRPC build time can be quite long depending on your build machine, you can experiment with faster build times by changing the 'make -j 1' calls, in *Dockerfile*, to a higher concurrency number

 * [1] clone this repository into a folder (*$my_build_folder*) on your build machine
 * [2] on your build machine open a terminal and change directory to *$my_build_folder*
 * [3] run: `$ docker build -t raft-rpc-docker .`
 * [4] confirm the image was built by running: `$ docker images`

# To start the RPC server
 * [1] run: `$ docker run --name=mysrvr --expose=50051 -it raft-rpc-docker /raft-rpc/RaftDateServer`
 * [2] confirm the message **"Server listening on: 172.17.0.2:50051"** is showing in the terminal window

# To run the client
 * [1] on your build machine open a new terminal and change directory to *$my_build_folder*
 * [2] run: `$ docker run --name=mycli -it raft-rpc-docker /raft-rpc/RaftDateClient`
 * [3] confirm the message **"Please enter two dates (MM/DD/YYYY)"** is showing in the terminal window
 * [4] in the terminal, next to the **"1)"** type in a date in the MM/DD/YYYY format and hit ENTER
 * [5] in the terminal, you should see a **"2)"**, type in another date in the MM/DD/YYYY format and hit ENTER
 * [6] if the client/server connection is successful, you should see a response back from the server that starts **"gRPC Returned:"** and the number of days difference between the date you entered in 1) and 2)
 * [7] the client will repeat starting at step 3 after each pair of dates is entered
