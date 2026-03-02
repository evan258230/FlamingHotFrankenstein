/**
 * @file WebServer.cpp
 * @brief Implementation file for the WebServer class.
 */

#include "WebServer.h"

/**
 * @brief Constructs a new WebServer, initially free and ready for requests.
 */
WebServer::WebServer() {
    free = true;
    requestStart = 0;
}

/**
 * @brief Returns whether the server is currently occupied.
 * @return True if processing a request, false otherwise.
 */
bool WebServer::busy() {
    return !free;
}

/**
 * @brief Assigns a Request to the server.
 * @param req The incoming Request.
 * @param currentTime The clock cycle when the assignment occurred.
 */
void WebServer::addRequest(Request req, int currentTime) {
    currentRequest = req;
    requestStart = currentTime;
    free = false; 
}

/**
 * @brief Processes the active request for one clock cycle.
 * @return True if the request completes during this cycle, freeing up the server.
 */
bool WebServer::tick() {
    if (free) {
        return false; 
    }
    
    currentRequest.time--; 
    
    if (currentRequest.time <= 0) {
        free = true; 
        return true; 
    }
    
    return false;
}

/**
 * @brief Gets the server's active request.
 * @return The Request object currently held by the server.
 */
Request WebServer::getRequest() {
    return currentRequest;
}