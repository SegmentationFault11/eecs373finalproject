/*
 * Written by Aaryaman Sagar (rmn100@gmail.com)
 *
 * This class is used to connect to an HTTP API Server and send it messages.
 * On construction this module does an HTTP handshake with the server to
 * establish another sub-server socket in the server.  The server responds
 * with an HTTP response with the port number encoded in JSON format.  This
 * object then knows which port to connect with on the given server.
 *
 * All future communication with the API server is done using the protocol
 * described below
 *  
 *  1. All messages sent to the API server must be of the following format
 *      <size_of_message><NULL><message>
 *     with the size_of_message denoting the total number of bytes that are
 *     going to be in the message including the size and the null character
 *  
 *  2. The message has to be in JSON format for easy processing on the server,
 *     which means that this will be used as an endpoint to the Events MySQL
 *     table and an object will be of the format 
 *     {"information": "<info>","timestamp": "<timestamp>"}
 */
#include <vector>
#include <string>
#include <cstdint>
using std::string;

class HttpApiConnector {
public:

    /*
     * Singleton pattern for this class.
     * Gets an connector to the API server, this object is going to be used to
     * send messages accross to the API server.  The message format is
     * described in the comments above.
     *
     * The get_connector() fails if one has not been initialized via a call to
     * get_connector(host, port)
     */
    static HttpApiConnector& get_connector(const string& host, const string& port);
    static HttpApiConnector& get_connector();

    /*
     * Starts off a new thread and makes a connection to the API server
     * specified by the host and port to the factory of the connector.  
     *
     * This function is non blocking.
     */
    void send_event_information(const string& information);

    /* Use to accept a message on a socket */
    void detach_set_callback(void (*callback) (std::vector<uint8_t>));

private:

    static HttpApiConnector* static_instance;
    HttpApiConnector(const string& host, const string& port);

    /* 
     * These variables are used to identify the new server that has been
     * "spawned"
     */
    int new_port_for_api_server;
    std::string new_host_for_api_server;
};
