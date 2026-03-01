#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "RequestQueue.h"
#include "WebServer.h"
#include <vector>
#include <string>
#include <fstream>


class LoadBalancer {
private:
    RequestQueue requestQueue;
    std::vector<WebServer> webServers;
    std::string blockedIPStart;
    std::string blockedIPEnd;

public:
    LoadBalancer(int serversCount);
    int queueSize();
    void addRequest(Request r, std::ofstream& logFile);
    void cycleStep();
    void balance(int currentTime, std::ofstream& logFile);
    void adjustServers();
    void setBlockedIPRange(std::string startIP, std::string endIP);
    void status();
    bool blocked(std::string ip);
};

#endif 