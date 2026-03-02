/**
 * @file LoadBalancer.h
 * @brief Header file for the LoadBalancer class.
 */

#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "RequestQueue.h"
#include "WebServer.h"
#include <vector>
#include <string>
#include <fstream>

/**
 * @class LoadBalancer
 * @brief Manages a dynamic cluster of WebServers, routing traffic and scaling hardware based on demand.
 */
class LoadBalancer {
private:
    RequestQueue requestQueue;            ///< The queue holding incoming, unassigned requests.
    std::vector<WebServer> webServers;    ///< Dynamic array of server nodes handling the traffic.
    std::string blockedIPStart;           ///< The starting IP address for the firewall block range.
    std::string blockedIPEnd;             ///< The ending IP address for the firewall block range.
    int rejectedRequests = 0;             ///< Counter for the total number of requests blocked by the firewall.

public:
    /**
     * @brief Constructor for the LoadBalancer.
     * @param serversCount The initial number of servers to allocate.
     */
    LoadBalancer(int serversCount);

    /**
     * @brief Gets the current number of requests waiting in the queue.
     * @return The queue size.
     */
    int queueSize();

    /**
     * @brief Receives an incoming request, checks the firewall, and adds it to the queue if allowed.
     * @param r The incoming Request object.
     * @param logFile The output file stream to log firewall events.
     */
    void addRequest(Request r, std::ofstream& logFile);

    /**
     * @brief Progresses time by calling tick() on all active servers.
     */
    void cycleStep();

    /**
     * @brief Distributes queued requests to any available, idle servers.
     * @param currentTime The current simulation clock cycle.
     * @param logFile The output file stream to log routing assignments.
     */
    void balance(int currentTime, std::ofstream& logFile);

    /**
     * @brief Dynamically scales the server cluster up or down based on current queue congestion.
     * @param logFile The output file stream to log scaling events.
     */
    void adjustServers(std::ofstream& logFile);

    /**
     * @brief Configures the firewall's IP block list.
     * @param startIP The start of the blocked IP subnet.
     * @param endIP The end of the blocked IP subnet.
     */
    void setBlockedIPRange(std::string startIP, std::string endIP);

    /**
     * @brief Prints a brief console summary of the queue and server status.
     */
    void status();

    /**
     * @brief Checks if an IP address is blocked by the current firewall configuration.
     * @param ip The IP string to check.
     * @return True if the IP is blocked, false otherwise.
     */
    bool blocked(std::string ip);

    /**
     * @brief Retrieves the total number of requests dropped by the firewall.
     * @return Number of rejected requests.
     */
    int getRejectedCount() { return rejectedRequests; }

    /**
     * @brief Retrieves the total number of servers currently active in the cluster.
     * @return Current server count.
     */
    int getServersCount() { return webServers.size(); }
};

#endif