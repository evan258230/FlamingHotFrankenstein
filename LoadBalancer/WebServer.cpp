#include "WebServer.h"

WebServer::WebServer() {
    free = true;
    requestStart = 0;
}

bool WebServer::busy() {
    return !free;
}


void WebServer::addRequest(Request req, int currentTime) {
    currentRequest = req;
    requestStart = currentTime;
    free = false; 
}


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

Request WebServer::getRequest() {
    return currentRequest;
}