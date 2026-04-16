/**
 * @file main.cpp
 * @brief The main driver file for the Flaming Hot Frankenstein Corp. Load Balancer Simulation.
 * * Sets up configuration settings, implements dual load balancers via a switch, generates network traffic,
 * handles dynamic server allocation, and writes comprehensive metrics to a log file.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "LoadBalancer.h"
#include "Request.h"

/**
 * @brief Helper function that generates a randomized IPv4 address string.
 * @return A string formatted as "X.X.X.X".
 */
std::string generateRandomIP() {
    return std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256);
}

/**
 * @brief Generates a random job request.
 * @param maxProcessTime The maximum possible time complexity for the generated request.
 * @return A randomized Request object.
 */
Request generateRandomRequest(int maxProcessTime) {
    std::string ipIn = generateRandomIP();
    std::string ipOut = generateRandomIP();
    int time = (rand() % maxProcessTime) + 1; 
    char type = (rand() % 2 == 0) ? 'P' : 'S'; 
    return Request(ipIn, ipOut, time, type);
}

/**
 * @brief Main execution function.
 * @return Exit status code.
 */
int main() {
    srand(time(0));
    
    // default configuration values 
    int numServers = 10;
    int runTime = 10000;
    int newRequestChance = 15;
    int maxProcessTime = 500;
    std::string blockStart = "120.0.0.0";
    std::string blockEnd = "125.255.255.255";

    //read config.txt
    std::ifstream configFile("config.txt");
    if (configFile.is_open()) {
        std::string line;
        while (std::getline(configFile, line)) {
            if (line.find("numServers=") == 0) numServers = std::stoi(line.substr(11));
            else if (line.find("runTime=") == 0) runTime = std::stoi(line.substr(8));
            else if (line.find("newRequestChance=") == 0) newRequestChance = std::stoi(line.substr(17));
            else if (line.find("maxProcessTime=") == 0) maxProcessTime = std::stoi(line.substr(15));
            else if (line.find("blockedIPStart=") == 0) blockStart = line.substr(15);
            else if (line.find("blockedIPEnd=") == 0) blockEnd = line.substr(13);
        }
        configFile.close();
        std::cout << "Successfully loaded config.txt\n";
    } else {
        std::cout << "Could not find config.txt. Using default settings.\n";
    }

    //setting up the log file
    std::ofstream logFile("log.txt");
    if (!logFile.is_open()) {
        std::cerr << "Error: Could not create log.txt!\n";
        return 1;
    }

    std::cout << "=========================================================\n";
    std::cout << "  Flaming Hot Frankenstein Corp. - Load Balancer System  \n";
    std::cout << "=========================================================\n\n";
    
    logFile << "=========================================================\n";
    logFile << "  Flaming Hot Frankenstein Corp. - Simulation Log        \n";
    logFile << "=========================================================\n";
    logFile << "Configuration: Total Servers=" << numServers << ", RunTime=" << runTime 
            << ", TrafficChance=" << newRequestChance << "%\n\n";

    //switch between processing servers and streaming P and S
    int processingServers = numServers / 2;
    int streamingServers = numServers - processingServers; 

    LoadBalancer lbProcessing(processingServers);
    LoadBalancer lbStreaming(streamingServers);

    std::cout << "[SWITCH] Splitting traffic into Processing (" << processingServers 
              << " servers) and Streaming (" << streamingServers << " servers) clusters.\n";
    logFile << "[SWITCH] Dual Load Balancers Activated.\n";

   //set firewall for both load balancers
    if (blockStart > blockEnd) {
        std::swap(blockStart, blockEnd);
    }
    lbProcessing.setBlockedIPRange(blockStart, blockEnd);
    lbStreaming.setBlockedIPRange(blockStart, blockEnd);

    int initialQueueSize = numServers * 100;
    std::cout << "Starting Total Queue Size: " << initialQueueSize << "\n"; 
    std::cout << "Task Time Range: 1 to " << maxProcessTime << " clock cycles\n"; 
    
    logFile << "Starting Total Queue Size: " << initialQueueSize << "\n";
    logFile << "Task Time Range: 1 to " << maxProcessTime << " cycles\n";

    for (int i = 0; i < initialQueueSize; i++) {
        Request newReq = generateRandomRequest(maxProcessTime);
        
        if (newReq.jobType == 'P') {
            lbProcessing.addRequest(newReq, logFile);
        } else {
            lbStreaming.addRequest(newReq, logFile);
        }
    }

    logFile << "[SYSTEM] Booting up simulation with " << initialQueueSize << " initial requests...\n";
    logFile << "[FIREWALL] Configured to block range: " << blockStart << " to " << blockEnd << "\n";
    logFile << "---------------------------------------------------------\n";
    int totalRequestsReceived = initialQueueSize;

    //main clock loop
    for (int currentTime = 0; currentTime < runTime; currentTime++) {
        
        //simulating random traffic chance
        if (rand() % 100 < newRequestChance) { 
            Request newReq = generateRandomRequest(maxProcessTime);
            
            if (newReq.jobType == 'P') {
                lbProcessing.addRequest(newReq, logFile);
            } else {
                lbStreaming.addRequest(newReq, logFile);
            }
            totalRequestsReceived++;
        }

        lbProcessing.balance(currentTime, logFile);
        lbStreaming.balance(currentTime, logFile);
        
        lbProcessing.cycleStep();
        lbStreaming.cycleStep();

        if (currentTime % 100 == 0) {
            lbProcessing.adjustServers(logFile);
            lbStreaming.adjustServers(logFile);
        }

        //logging every 1000 clock ticks to prevent too much logging
        if (currentTime % 1000 == 0) {
            logFile << "Clock Cycle: " << currentTime 
                    << " | P-Queue: " << lbProcessing.queueSize() 
                    << " | S-Queue: " << lbStreaming.queueSize() << "\n";
        }
    }
    int finalServersProcessing = lbProcessing.getServersCount();
    int finalServersStreaming = lbStreaming.getServersCount();
    int totalFinalServers = finalServersProcessing + finalServersStreaming;
    
    int totalRemainingQueue = lbProcessing.queueSize() + lbStreaming.queueSize();
    int totalRejected = lbProcessing.getRejectedCount() + lbStreaming.getRejectedCount();

    std::stringstream finalStats;
    finalStats << "\n=========================================================\n"
               << "                   FINAL SYSTEM STATUS                    \n"
               << "=========================================================\n"
               << "Ending Total Queue Size: " << totalRemainingQueue << "\n" 
               << "   -> Processing Queue : " << lbProcessing.queueSize() << "\n"
               << "   -> Streaming Queue  : " << lbStreaming.queueSize() << "\n"
               << "Total Requests Received: " << totalRequestsReceived << "\n"
               << "Requests Rejected (FW) : " << totalRejected << "\n" 
               << "Total Servers at End   : " << totalFinalServers << "\n"
               << "   -> Processing Nodes : " << finalServersProcessing << "\n"
               << "   -> Streaming Nodes  : " << finalServersStreaming << "\n"
               << "Total Clock Cycles     : " << runTime << "\n"
               << "=========================================================\n";

    std::cout << finalStats.str();
    logFile << finalStats.str();

    logFile.close();
    std::cout << "[SYSTEM] Simulation Complete. Output saved to log.txt.\n";
    
    return 0;
}