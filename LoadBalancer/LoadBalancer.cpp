#include "LoadBalancer.h"
#include <iostream>

LoadBalancer::LoadBalancer(int serversCount) {
    blockedIPStart = ""; 
    blockedIPEnd = "";
    
    for (int i = 0; i < serversCount; i++) {
        webServers.push_back(WebServer());
    }
}

int LoadBalancer::queueSize() {
    return requestQueue.size();
}

void LoadBalancer::addRequest(Request r, std::ofstream& logFile) {
    if (!blocked(r.ipIn)) {
        requestQueue.push(r);
    } else {
        std::cout << "\033[1;31m[FIREWALL]\033[0m Blocked malicious request from IP: " << r.ipIn << "\n";
        logFile << "[FIREWALL] Blocked malicious request from IP: " << r.ipIn << "\n";
    }
}

void LoadBalancer::cycleStep() {
    for (size_t i = 0; i < webServers.size(); i++) {
        webServers[i].tick();
    }
}

void LoadBalancer::balance(int currentTime, std::ofstream& logFile) {
    for (size_t i = 0; i < webServers.size(); i++) {
        if (!webServers[i].busy() && !requestQueue.isEmpty()) {
            Request nextReq = requestQueue.pop();
            webServers[i].addRequest(nextReq, currentTime);
            
            std::string logMsg = "Time: " + std::to_string(currentTime) + 
                                 " | Routed IP: " + nextReq.ipIn + 
                                 " -> Server [" + std::to_string(i) + "]\n";
            
            logFile << logMsg;
            
        }
    }
}

void LoadBalancer::adjustServers() {
    int currentQueueSize = requestQueue.size();
    int currentServers = webServers.size();
    
    if (currentServers == 0) {
        return;
    }
    
    if (currentQueueSize > currentServers * 80) {
        webServers.push_back(WebServer());
        std::cout << "\033[1;33m[SCALE UP]\033[0m Added a server. Total servers: " << webServers.size() << "\n";
    } 
    else if (currentQueueSize < currentServers * 50 && currentServers > 1) {
        webServers.pop_back(); 
        std::cout << "\033[1;36m[SCALE DOWN]\033[0m Removed a server. Total servers: " << webServers.size() << "\n";
    }
}

void LoadBalancer::status() {
    int currentServers = webServers.size();
    int busyServers = 0;
    
    for (int i = 0; i < currentServers; i++) {
        if (webServers[i].busy()) {
            busyServers++;
        }
    }
    
    std::cout << "Queue: " << requestQueue.size() 
              << " | Servers: " << currentServers 
              << " (Active: " << busyServers << ", Idle: " << (currentServers - busyServers) << ")\n";
}

void LoadBalancer::setBlockedIPRange(std::string startIP, std::string endIP) {
    blockedIPStart = startIP;
    blockedIPEnd = endIP;
}

bool LoadBalancer::blocked(std::string ip) {
    if (blockedIPStart.empty() || blockedIPEnd.empty()) {
        return false;
    }
    if (ip >= blockedIPStart && ip <= blockedIPEnd) {
        return true;
    }
    
    return false;
}

