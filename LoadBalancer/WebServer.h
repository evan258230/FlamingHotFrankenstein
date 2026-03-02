/**
 * @file WebServer.h
 * @brief Header file for the WebServer class.
 */

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

/**
 * @class WebServer
 * @brief Simulates a single server node that processes individual requests over time.
 */
class WebServer {
private:
    Request currentRequest; ///< The request currently being processed by the server.
    int requestStart;       ///< The clock cycle when the server started processing the current request.
    bool free;              ///< Boolean indicating if the server is available for new work.

public:
    /**
     * @brief Default constructor for WebServer. Initializes as free.
     */
    WebServer();

    /**
     * @brief Checks if the server is currently processing a request.
     * @return True if busy, false if available.
     */
    bool busy();

    /**
     * @brief Assigns a new request to the server and marks it as busy.
     * @param req The request to process.
     * @param currentTime The current clock cycle when the request was assigned.
     */
    void addRequest(Request req, int currentTime);

    /**
     * @brief Advances the server's processing state by one clock cycle.
     * @return True if a request just finished processing on this tick, false otherwise.
     */
    bool tick();

    /**
     * @brief Retrieves the request currently being processed.
     * @return The active Request object.
     */
    Request getRequest();
};

#endif