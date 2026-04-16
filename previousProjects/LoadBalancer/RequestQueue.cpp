/**
 * @file RequestQueue.cpp
 * @brief Implementation file for the RequestQueue class.
 */

#include "RequestQueue.h"

/**
 * @brief Adds a Request to the back of the queue.
 * @param req The Request to add.
 */
void RequestQueue::push(Request req) {
    requestQueue.push(req);
}

/**
 * @brief Retrieves and removes the next Request from the queue.
 * @return The Request at the front of the queue.
 */
Request RequestQueue::pop() {
    if (requestQueue.empty()) {
        return Request(); // return an empty request if queue is already empty
    }
    Request frontReq = requestQueue.front();
    requestQueue.pop();
    return frontReq;
}

/**
 * @brief Checks if the queue is empty.
 * @return True if empty, false if there are requests waiting.
 */
bool RequestQueue::isEmpty() {
    return requestQueue.empty();
}

/**
 * @brief Gets the current size of the queue.
 * @return The number of items in the queue.
 */
int RequestQueue::size() {
    return requestQueue.size();
}