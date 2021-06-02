#include <algorithm>
#include <grpcpp/grpcpp.h>

#include "protos/raft-date-server.grpc.pb.h"

/**
 * RaftDateServerImpl
 */
class RaftDateServerImpl final : public raft::date::RaftDate::Service
{
    private:
        static const long _one_day_seconds = 86400;

    public:

        /**
         * GetDateDiffInDays
         */
        grpc::Status GetDateDiffInDays(
            grpc::ServerContext* context,
            const raft::date::DateDiffRequest* request,
            raft::date::DateDiffResponse* response)
        {
            google::protobuf::int64
                start_date_seconds = request->startdate();

            google::protobuf::int64
                end_date_seconds = request->enddate();

            google::protobuf::int64
                date_diff_millis = 
                    ( std::max(start_date_seconds, end_date_seconds) -
                      std::min(start_date_seconds, end_date_seconds) );

            response->set_diffindays( ( date_diff_millis / _one_day_seconds ) );

            return grpc::Status::OK;
        }
};

/**
 * RunServer
 */
void RunServer()
{
    // NOTE: hard-coded string == bad
    std::string server_address("172.17.0.2:50051");

    RaftDateServerImpl service;

    grpc::ServerBuilder builder;

    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    std::cout << "Server listening on: " << server_address << std::endl;

    server->Wait();
}

/**
 * Main
 */
int main(int argc, char** argv)
{
    RunServer();

    return 0;
}