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

void LoadBalancer::addRequest(Request r) {
    if (!blocked(r.ipIn)) {
        requestQueue.push(r);
    } else {
       //log that an IP was blocked
    }
}

void LoadBalancer::cycleStep() {
    for (size_t i = 0; i < webServers.size(); i++) {
        webServers[i].tick();
    }
}

void LoadBalancer::balance(int currentTime) {
    for (size_t i = 0; i < webServers.size(); i++) {
        if (!webServers[i].busy() && !requestQueue.isEmpty()) {
            Request nextReq = requestQueue.pop();
            webServers[i].addRequest(nextReq, currentTime);
        }
    }
}

void LoadBalancer::adjustServers() {

}

void LoadBalancer::status() {
  
}

bool LoadBalancer::blocked(std::string ip) {
}