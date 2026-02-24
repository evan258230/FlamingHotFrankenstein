#include "RequestQueue.h"

void RequestQueue::push(Request req) {
    requestQueue.push(req);
}
Request RequestQueue::pop() {
    if (requestQueue.empty()) {
        return Request(); // return  an empty request if queue is already empty
    }
    Request frontReq = requestQueue.front();
    requestQueue.pop();
    return frontReq;
}

bool RequestQueue::isEmpty() {
    return requestQueue.empty();
}


int RequestQueue::size() {
    return requestQueue.size();
}