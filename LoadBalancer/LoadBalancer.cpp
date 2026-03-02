/**
 * @file LoadBalancer.cpp
 * @brief Implementation file for the LoadBalancer class.
 */

#include "LoadBalancer.h"
#include <iostream>

/**
 * @brief Initializes the LoadBalancer with a specified number of servers.
 * @param serversCount Number of initial WebServer instances to create.
 */
LoadBalancer::LoadBalancer(int serversCount) {
    blockedIPStart = ""; 
    blockedIPEnd = "";
    
    for (int i = 0; i < serversCount; i++) {
        webServers.push_back(WebServer());
    }
}

/**
 * @brief Returns the size of the request queue.
 * @return Integer size of the queue.
 */
int LoadBalancer::queueSize() {
    return requestQueue.size();
}

/**
 * @brief Adds a request to the queue unless blocked by the firewall.
 * @param r The Request to evaluate.
 * @param logFile Output stream for firewall logging.
 */
void LoadBalancer::addRequest(Request r, std::ofstream& logFile) {
    if (!blocked(r.ipIn)) {
        requestQueue.push(r);
    } else {
        rejectedRequests++; 
        std::cout << "\033[1;31m[FIREWALL]\033[0m Blocked malicious request from IP: " << r.ipIn << "\n";
        logFile << "[FIREWALL] Blocked malicious request from IP: " << r.ipIn << "\n";
    }
}

/**
 * @brief Ticks all active web servers by one clock cycle.
 */
void LoadBalancer::cycleStep() {
    for (size_t i = 0; i < webServers.size(); i++) {
        webServers[i].tick();
    }
}

/**
 * @brief Pulls waiting requests from the queue and assigns them to idle servers.
 * @param currentTime The current clock cycle.
 * @param logFile Output stream for routing logs.
 */
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

/**
 * @brief Dynamically adjusts the number of servers based on queue size thresholds.
 * @param logFile Output stream for scaling event logs.
 */
void LoadBalancer::adjustServers(std::ofstream& logFile) {
    int currentQueueSize = requestQueue.size();
    int currentServers = webServers.size();
    
    if (currentServers == 0) { return; }
    
    if (currentQueueSize > currentServers * 80) { //if above * 80, add one server
        webServers.push_back(WebServer());
        std::cout << "\033[1;33m[SCALE UP]\033[0m Added a server. Total servers: " << webServers.size() << "\n";
        logFile << "[SCALE UP] Added a server. Total servers: " << webServers.size() << "\n";
    } 
    else if (currentQueueSize < currentServers * 50 && currentServers > 1) { //if below * 50, remove 1 server
        webServers.pop_back(); 
        std::cout << "\033[1;36m[SCALE DOWN]\033[0m Removed a server. Total servers: " << webServers.size() << "\n";
        logFile << "[SCALE DOWN] Removed a server. Total servers: " << webServers.size() << "\n";
    }
}

/**
 * @brief Outputs the current status of the load balancer to the console.
 */
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

/**
 * @brief Updates the IP string boundaries for the firewall.
 * @param startIP The lower bound IP address.
 * @param endIP The upper bound IP address.
 */
void LoadBalancer::setBlockedIPRange(std::string startIP, std::string endIP) {
    blockedIPStart = startIP;
    blockedIPEnd = endIP;
}

/**
 * @brief Evaluates whether an IP address falls within the firewall's blocked range.
 * @param ip The IP address to test.
 * @return True if blocked, false if permitted.
 */
bool LoadBalancer::blocked(std::string ip) {
    if (blockedIPStart.empty() || blockedIPEnd.empty()) {
        return false;
    }
    if (ip >= blockedIPStart && ip <= blockedIPEnd) {
        return true;
    }
    
    return false;
}