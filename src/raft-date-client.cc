#include <ctime>
#include <grpcpp/grpcpp.h>
#include <sstream>

#include "protos/raft-date-server.grpc.pb.h"

namespace
{
    static const long _one_day_seconds = 86400;
}

/**
 * RaftDateClient
 */
class RaftDateClient
{
    public:

        /**
         * RaftDateClient
         */
        RaftDateClient(std::shared_ptr<grpc::Channel> channel) :
            _stub(raft::date::RaftDate::NewStub(channel))
        {
        }

        /**
         * GetDateDiffInDays
         */
        int64_t GetDateDiffInDays(
            std::time_t& start_date, 
            std::time_t& end_date)
        {
            raft::date::DateDiffRequest
                date_diff_request;

            date_diff_request.set_startdate(start_date);
            date_diff_request.set_enddate(end_date);

            raft::date::DateDiffResponse
                date_diff_response;

            grpc::ClientContext
                context;

            grpc::Status
                status = _stub->GetDateDiffInDays(
                    &context, 
                    date_diff_request, 
                    &date_diff_response);
            
            if (status.ok())
            {
                return date_diff_response.diffindays();
            }
            else
            {
                std::cout << status.error_code() 
                          << ": " << status.error_message()
                          << std::endl;

                return -1;
            }
        }

    private:

        std::unique_ptr<raft::date::RaftDate::Stub> _stub;

};

int main(int argc, char** argv)
{
    while (true)
    {
        std::string
            first_date,
            second_date;

        std::cout << "Please enter two dates (MM/DD/YYYY):" << std::endl;
        std::cout << "1) ";
        std::cin >> first_date;
        std::cout << "2) ";
        std::cin >> second_date;

        std::tm
            first_tm,
            second_tm;

        memset(&first_tm, 0, sizeof(std::tm));
        memset(&second_tm, 0, sizeof(std::tm));

        strptime(first_date.c_str(), "%0m/%0d/%Y", &first_tm);
        strptime(second_date.c_str(), "%0m/%0d/%Y", &second_tm);

        time_t
            first_seconds = mktime(&first_tm);

        time_t
            second_seconds = mktime(&second_tm);

        // NOTE: hard-coded string == bad
        // NOTE: no authentication mechanism
        RaftDateClient
            client(grpc::CreateChannel(
                "172.17.0.2:50051", 
                grpc::InsecureChannelCredentials()));

        int64_t
            day_diff = client.GetDateDiffInDays(first_seconds, second_seconds);

        std::cout << "gRPC returned:" << std::endl << day_diff << std::endl;
    }

    return 0;
}