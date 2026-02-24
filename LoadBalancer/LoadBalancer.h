#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "RequestQueue.h"
#include "WebServer.h"
#include <vector>
#include <string>


class LoadBalancer {
private:
    RequestQueue requestQueue;
    std::vector<WebServer> webServers;
    std::string blockedIPStart;
    std::string blockedIPEnd;

public:
    LoadBalancer(int serversCount);
    int queueSize();
    void addRequest(Request r);
    void cycleStep();
    void balance(int currentTime);
    void adjustServers();
    void status();
    bool blocked(std::string ip);
};

#endif 