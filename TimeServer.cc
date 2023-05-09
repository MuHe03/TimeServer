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

class TimeServiceImpl final : public TimeService::Service {
    grpc::Status GetTime(grpc::ServerContext *context, const TimeRequest *request, TimeResponse *response) override {
        auto utc = boost::posix_time::microsec_clock::universal_time();
        ptime ret;
        switch (request->time_zone()) {
            case TimeRequest_TimeZone::TimeRequest_TimeZone_UTC:
                ret = utc;
                break;
            case TimeRequest_TimeZone::TimeRequest_TimeZone_CST:
                ret = local_adjustor<ptime, -6, us_dst>::utc_to_local(utc);
                break;
            case TimeRequest_TimeZone::TimeRequest_TimeZone_EST:
                ret = local_adjustor<ptime, -5, us_dst>::utc_to_local(utc);
                break;
            case TimeRequest_TimeZone::TimeRequest_TimeZone_MST:
                ret = local_adjustor<ptime, -7, us_dst>::utc_to_local(utc);
                break;
            case TimeRequest_TimeZone::TimeRequest_TimeZone_PST:
                ret = local_adjustor<ptime, -8, us_dst>::utc_to_local(utc);
                break;
            default:
                return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Invalid TimeZone");
        }
        response->set_time(to_simple_string(ret));
        return grpc::Status::OK;
    }
};

void RunServer(int16_t port) {
    std::string server_address = "0.0.0.0:" + std::to_string(port);
    TimeServiceImpl service;

    grpc::EnableDefaultHealthCheckService(true);
    grpc::ServerBuilder builder;

    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char** argv) {
    std::cout << "TimeServer" << std::endl;
    // the first argument will be a String of the port number
    if (argc != 2) {
        std::cout << "Usage: ./TimeServer <port>" << std::endl;
        exit(1);
    }
    int16_t port = std::stoi(argv[1]);
    RunServer(port);
    return 0;
}