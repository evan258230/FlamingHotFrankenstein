#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "LoadBalancer.h"
#include "Request.h"

std::string generateRandomIP() {
    return std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256);
}

Request generateRandomRequest(int maxProcessTime) {
    std::string ipIn = generateRandomIP();
    std::string ipOut = generateRandomIP();
    int time = (rand() % maxProcessTime) + 1; 
    char type = (rand() % 2 == 0) ? 'P' : 'S'; 
    return Request(ipIn, ipOut, time, type);
}

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
    logFile << "Configuration: Servers=" << numServers << ", RunTime=" << runTime 
            << ", TrafficChance=" << newRequestChance << "%\n\n";

    LoadBalancer lb(numServers);
    

    //setting up the firewall based on cofig file
    if (blockStart > blockEnd) {
        std::swap(blockStart, blockEnd);
    }
    lb.setBlockedIPRange(blockStart, blockEnd);

    int initialQueueSize = numServers * 100;
    for (int i = 0; i < initialQueueSize; i++) {
        lb.addRequest(generateRandomRequest(maxProcessTime), logFile);
    }

    logFile << "[SYSTEM] Booting up simulation with " << initialQueueSize << " initial requests...\n";
    logFile << "[FIREWALL] Configured to block range: " << blockStart << " to " << blockEnd << "\n";
    logFile << "---------------------------------------------------------\n";
    int totalRequestsReceived = initialQueueSize;

    //main clock loop
    for (int currentTime = 0; currentTime < runTime; currentTime++) {
        
        //simulating random traffic chance
        if (rand() % 100 < newRequestChance) { 
            lb.addRequest(generateRandomRequest(maxProcessTime), logFile);
            totalRequestsReceived++;
        }

        lb.balance(currentTime, logFile);
        lb.cycleStep();

        if (currentTime % 100 == 0) {
            lb.adjustServers();
        }

        //logging every 1000 clock ticks to prevent too much logging
        if (currentTime % 1000 == 0) {
            logFile << "Clock Cycle: " << currentTime << " | Queue Size: " << lb.queueSize() << "\n";
        }
    }

    std::string summary = "\n=========================================================\n"
                          "                   SIMULATION SUMMARY                    \n"
                          "=========================================================\n"
                          "Total Clock Cycles Run : " + std::to_string(runTime) + "\n"
                          "Total Requests Received: " + std::to_string(totalRequestsReceived) + "\n"
                          "Final Queue Size       : " + std::to_string(lb.queueSize()) + "\n"
                          "=========================================================\n";

    std::cout << summary;
    logFile << summary;

    logFile.close();
    std::cout << "[SYSTEM] Simulation Complete. Output saved to log.txt.\n";
    
    return 0;
}