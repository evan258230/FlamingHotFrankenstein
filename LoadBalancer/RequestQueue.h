#ifndef REQUESTQUEUE_H
#define REQUESTQUEUE_H

#include "Request.h"
#include <queue>

/**
 * @class RequestQueue
 * @brief Manages the queue of incoming requests for the load balancer.
 */
class RequestQueue {
private:
    std::queue<Request> requestQueue;

public:
    /**
     * @brief Pushes a new request to the back of the queue.
     * @param req The Request object to add.
     */
    void push(Request req);

    /**
     * @brief Removes and returns the request at the front of the queue.
     * @return The next Request object.
     */
    Request pop();

    /**
     * @brief Checks if the queue is currently empty.
     * @return True if empty, false otherwise.
     */
    bool isEmpty();

    /**
     * @brief Returns the current number of items in the queue.
     * @return Integer representing the queue size.
     */
    int size();
};

#endif 