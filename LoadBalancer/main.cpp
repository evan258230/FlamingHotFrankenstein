#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "LoadBalancer.h"
#include "Request.h"
#include "RequestQueue.h"
#include "WebServer.h"

/**
 * @brief Helper function to generate a random IPv4 address.
 * @return A string representing an IP address.
 */
std::string generateRandomIP() {
    return std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256);
}

/**
 * @brief Helper function to generate a random Request.
 * @return A Request object with random IPs, processing time, and job type.
 */
Request generateRandomRequest() {
    std::string ipIn = generateRandomIP();
    std::string ipOut = generateRandomIP();
    int time = (rand() % 500) + 1; // Random processing time between 1 and 500 cycles
    char type = (rand() % 2 == 0) ? 'P' : 'S'; // 50/50 chance for Processing or Streaming
    return Request(ipIn, ipOut, time, type);
}

int main() {
    // Seed the random number generator
    srand(time(0));
    
    int numServers;
    int runTime;

    std::cout << "=========================================================\n";
    std::cout << "  Flaming Hot Frankenstein Corp. - Load Balancer System  \n";
    std::cout << "=========================================================\n\n";

    // 1. User Input for initial setup
    std::cout << "Enter initial number of servers (e.g., 10): ";
    std::cin >> numServers;
    std::cout << "Enter time to run (in clock cycles, e.g., 10000): ";
    std::cin >> runTime;

    LoadBalancer lb(numServers);

    // 2. Generate full initial queue (servers * 100)
    int initialQueueSize = numServers * 100;
    for (int i = 0; i < initialQueueSize; i++) {
        lb.addRequest(generateRandomRequest());
    }

    std::cout << "\n[SYSTEM] Booting up simulation with " << initialQueueSize << " initial requests...\n";
    std::cout << "---------------------------------------------------------\n";

    // 3. Main Simulation Loop
    for (int currentTime = 0; currentTime < runTime; currentTime++) {
        
        // Simulate incoming network traffic
        // e.g., A 10% chance to receive a new request on any given clock cycle
        if (rand() % 100 < 10) { 
            lb.addRequest(generateRandomRequest());
        }

        // The load balancer hands out queued jobs to free servers
        lb.balance(currentTime);

        // Process 1 clock cycle for all active servers
        lb.cycleStep();

        // Adjust servers to maintain the 50x - 80x queue ratio
        // We only check this every 100 cycles to prevent the system from thrashing
        if (currentTime % 100 == 0) {
            lb.adjustServers();
        }

        // Output logging periodically so the console isn't flooded
        if (currentTime % 1000 == 0) {
            std::cout << "Clock Cycle: " << currentTime << " | ";
            lb.status(); 
        }
    }

    std::cout << "---------------------------------------------------------\n";
    std::cout << "[SYSTEM] Simulation Complete. Shutting down gracefully.\n";
    
    return 0;
}