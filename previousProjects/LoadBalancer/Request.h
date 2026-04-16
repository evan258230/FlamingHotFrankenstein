/**
 * @file Request.h
 * @brief Header file for the Request class.
 */

#ifndef REQUEST_H
#define REQUEST_H
#include <string>

/**
 * @class Request
 * @brief Represents a single job request with IP tracking, processing time, and job type.
 */
class Request {
public:
    std::string ipIn;  ///< The origin IP address.
    std::string ipOut; ///< The destination IP address.
    int time;          ///< Time required to process the request (in clock cycles).
    char jobType;      ///< 'P' for Processing, 'S' for Streaming.

    /**
     * @brief Default constructor for Request. Initializes with empty strings and 0 time.
     */
    Request();

    /**
     * @brief Parameterized constructor to initialize a new Request.
     * @param in The origin IP address.
     * @param out The destination IP address.
     * @param t Time required to process the request (in clock cycles).
     * @param type The type of job ('P' or 'S').
     */
    Request(std::string in, std::string out, int t, char type);
};

#endif