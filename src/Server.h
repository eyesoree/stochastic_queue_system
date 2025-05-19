#ifndef SERVER_H
#define SERVER_H

#include <iostream>

enum class ServerStatus {
    BUSY,
    IDLE
};

struct Server {
    size_t id = 0;
    ServerStatus status = ServerStatus::IDLE;
    int totalCustomer = 0;     // Total customers served by this server
    double totalBusyTime = 0;  // Total time this server was busy
};

#endif