#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"
class WebServer {
private:
    Request currentRequest;
    int requestStart;
    bool free;

public:
    WebServer();
    bool busy();
    void addRequest(Request req, int currentTime);
    bool tick();
    Request getRequest();
};

#endif 