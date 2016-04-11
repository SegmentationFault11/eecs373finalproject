#include "HttpApiConnector.hpp"
#include <cppsockets/SocketUtilities.hpp>
#include <cppsockets/SocketRAII.hpp>
#include <array>
#include <sstream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <algorithm>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::stringstream;

/* initialize statics */
HttpApiConnector* HttpApiConnector::static_instance = nullptr;

/* Does the handshake with the server and return a vector with the response */
static vector<char> do_handshake(int api_server_socket);
/* Used to parse out the port number from a response from the api server */
static int parse_port_from_api_server_response(const vector<char>& response);

HttpApiConnector& HttpApiConnector::get_connector(const string& host, 
        const string& port) {

    // create the singleton if one does not exist
    if (!HttpApiConnector::static_instance) {
        HttpApiConnector::static_instance = new HttpApiConnector {host, port};
    }
    return *HttpApiConnector::static_instance;
}

HttpApiConnector::HttpApiConnector(const string& host, const string& port) {

    // open a socket with the API server
    using SocketUtilities::SocketRAII;
    SocketRAII api_server_socket = SocketUtilities::create_client_socket(
            host.c_str(), port.c_str());

    // do the handshake and print the result of the handshake
    auto handshake_response = do_handshake(api_server_socket);
    cout << "The new port number for the server is " << 
        parse_port_from_api_server_response(handshake_response) << endl;
}

vector<char> do_handshake(int api_server_socket) {

    // The HTTP handshake request to be sent to the API server 
    static const string handshake_request = "GET /open_api_connection HTTP/1.1\n"
    "Host: localhost:8000\n"
    "User-Agent: curl/7.43.0\n"
    "Accept: */*\n\n";

    // send the initial request to the server
    const vector<char> handshake_data (handshake_request.begin(), 
        handshake_request.end());
    SocketUtilities::send_all(api_server_socket, handshake_data);

    // receive the JSON response, TODO I have used a bad technique for this
    // receive, is it reliable?  Probably not.  I should look into ways to
    // change this later on.  Try catch blocks should not be used to dictate
    // logic
    std::array<char, 1024> recv_buffer;
    int total_bytes_received {0};
    while(true) {
        auto bytes_received = ::recv(api_server_socket, 
                recv_buffer.data() + total_bytes_received, recv_buffer.size(), 0);

        // break if 0 bytes were received, throw error if -1 returned
        total_bytes_received += bytes_received;
        if (!bytes_received) {
            break;
        } else if (bytes_received == -1) {
            throw std::runtime_error("Could not establish handshake with server");
        }
    }

    return vector<char> (recv_buffer.data(), 
            recv_buffer.data() + total_bytes_received);
}

int parse_port_from_api_server_response(const vector<char>& response) {
    
    stringstream ss_used_to_parse_json;
    string string_response (response.begin(), response.end());
    ss_used_to_parse_json << string_response;

    // take out the crap, i.e. read till the ':' in JSON and then read in the
    // integer.  This is because the JSON is of the form
    //     {"port":8080}
    int port_number_for_new_server;
    getline(ss_used_to_parse_json, string_response, '{');
    getline(ss_used_to_parse_json, string_response, ':');
    ss_used_to_parse_json >> port_number_for_new_server;

    // if reading in the port number failed then throw a runtime error
    if (!ss_used_to_parse_json) {
        throw std::runtime_error("Could not parse out the new port number for "
                "the sub-API server");
    }
    return port_number_for_new_server;
}
