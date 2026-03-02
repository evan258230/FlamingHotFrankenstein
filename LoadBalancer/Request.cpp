/**
 * @file Request.cpp
 * @brief Implementation file for the Request class.
 */

#include "Request.h"

/**
 * @brief Constructs a default Request object with empty IPs, 0 time, and type 'P'.
 */
Request::Request() {
    ipIn = "";
    ipOut = "";
    time = 0;
    jobType = 'P';
}

/**
 * @brief Constructs a Request with specific parameters.
 * @param in Origin IP address.
 * @param out Destination IP address.
 * @param t Processing time required.
 * @param type Job type classification ('P' or 'S').
 */
Request::Request(std::string in, std::string out, int t, char type) {
    ipIn = in;
    ipOut = out;
    time = t;
    jobType = type;
}