#ifndef REQUEST_H
#define REQUEST_H
#include <string>

/**
 * @class Request
 * @brief Represents a single job request with IP tracking, processing time, and job type.
 */
class Request {
public:
    std::string ipIn;
    std::string ipOut;
    int time;
    char jobType; // 'P' for Processing, 'S' for Streaming

    /**
     * @brief Default constructor for Request.
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