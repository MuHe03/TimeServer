#include <bits/stdc++.h>
#include <grpcpp/grpcpp.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>

#include "Time.pb.h"
#include "Time.grpc.pb.h"

using TimeServer::TimeRequest;
using TimeServer::TimeRequest_TimeZone;
using TimeServer::TimeResponse;
using TimeServer::TimeService;
using namespace boost::gregorian;
using namespace boost::date_time;
using namespace boost::posix_time;

class TimeClient {
    public:
        TimeClient(std::shared_ptr<grpc::Channel> channel) : stub_(TimeService::NewStub(channel)) {}

        std::string GetTime(TimeRequest_TimeZone time_zone) {
            TimeRequest request;
            request.set_time_zone(time_zone);
            TimeResponse response;
            grpc::ClientContext context;
            grpc::Status status = stub_->GetTime(&context, request, &response);
            if (status.ok()) {
                return response.time();
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
                return "RPC failed";
            }
        }

    private:
        std::unique_ptr<TimeService::Stub> stub_;
};

int main(int argc, char** argv) {
    // the first argument will be a String of the time zone
    // the second argument will be a String of the port number
    if (argc != 3) {
        std::cout << "Usage: ./TimeClient <time_zone> <port>" << std::endl;
        exit(1);
    }
    std::string time_zone = argv[1];
    int16_t port = std::stoi(argv[2]);
    TimeClient client(grpc::CreateChannel("localhost:" + std::to_string(port), grpc::InsecureChannelCredentials()));
    TimeRequest_TimeZone tz;
    switch (time_zone[0]) {
        case 'U':
            tz = TimeRequest_TimeZone::TimeRequest_TimeZone_UTC;
            break;
        case 'C':
            tz = TimeRequest_TimeZone::TimeRequest_TimeZone_CST;
            break;
        case 'E':
            tz = TimeRequest_TimeZone::TimeRequest_TimeZone_EST;
            break;
        case 'M':
            tz = TimeRequest_TimeZone::TimeRequest_TimeZone_MST;
            break;
        case 'P':
            tz = TimeRequest_TimeZone::TimeRequest_TimeZone_PST;
            break;
        default:
            std::cout << "Invalid TimeZone" << std::endl;
            exit(1);
    }
    std::string reply = client.GetTime(tz);
    std::cout << "Time received: " << reply << std::endl;
    
    return 0;
}