#include "Request.h"

Request::Request() {
    ipIn = "";
    ipOut = "";
    time = 0;
    jobType = 'P';
}

Request::Request(std::string in, std::string out, int t, char type) {
    ipIn = in;
    ipOut = out;
    time = t;
    jobType = type;
}