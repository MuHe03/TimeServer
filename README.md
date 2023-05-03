# TimeServer
## Introduction
This repository will implement a time server/client which allow the client to send asks to server to ask current local time and get suitable response based on the time zone. 

The project will based on [gRPC](https://grpc.io/) protocol and the main part will be implementing the communicating protocol between server and client using the gRPC framework. 

The main programming language will be C++. 

## Features and Goals
### Requirements

1. The server should be implemented using the gRPC framework.
2. The server should support queries for the current time in at least three different time zones.
3. The client should be implemented using the gRPC framework.
4. The client should allow the user to select a time zone and request the current time in that time zone.
5. The client should display the current time received from the server in a human-readable format.

### Instructions

1. Create a new gRPC project using the gRPC tools provided by the gRPC framework.
2. Define a gRPC service that allows clients to query the server for the current time in a specified time zone.
3. Implement the gRPC service in the server application.
4. Implement the client application using the gRPC client library provided by the gRPC framework.
5. Test your client-server application to ensure it works as expected.

### Bonus

1. Add support for additional time zones.
2. Implement error handling in the client and server applications.
3. Use a database or external service to retrieve the current time in each time zone.
4. Implement authentication and authorization for client requests to the server.