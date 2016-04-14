#include "HttpApiConnector.hpp"
#include "SerialDataParser.hpp"
#include <cppsockets/SocketUtilities.hpp>
#include <cppsockets/SocketRAII.hpp>
#include <array>
#include <sstream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <thread>
#include <algorithm>
#include <unistd.h>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <map>
#include <algorithm>
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::stringstream;
using std::map;

/* initialize statics */
HttpApiConnector* HttpApiConnector::static_instance = nullptr;

/* Does the handshake with the server and return a vector with the response */
static vector<char> do_handshake(int api_server_socket);
/* Used to parse out json from a response from the api server */
static map<string, string> parse_json_from_api_server_response(
        const vector<char>& response);

/* Used to send data over to the address and port received from the server */
void send_to_server(string address, string port, 
        string information, string timestamp);

HttpApiConnector& HttpApiConnector::get_connector(const string& host, 
        const string& port) {

    // create the singleton if one does not exist
    if (!HttpApiConnector::static_instance) {
        HttpApiConnector::static_instance = new HttpApiConnector {host, port};
    }
    return *HttpApiConnector::static_instance;
}

HttpApiConnector& HttpApiConnector::get_connector() {

    assert(HttpApiConnector::static_instance);
    return *HttpApiConnector::static_instance;
}

HttpApiConnector::HttpApiConnector(const string& host, const string& port) {

    // open a socket with the API server
    using SocketUtilities::SocketRAII;
    SocketRAII api_server_socket = SocketUtilities::create_client_socket(
            host.c_str(), port.c_str());

    // do the handshake and print the result of the handshake
    auto handshake_response_buffer = do_handshake(api_server_socket);
    auto json = parse_json_from_api_server_response(handshake_response_buffer);
    this->new_host_for_api_server = json["host"];
    this->new_port_for_api_server = stoi(json["port"]);
    cout << "The new host is [" << this->new_host_for_api_server 
        << "]" << endl;
    cout << "The new port is [" << this->new_port_for_api_server << "]" << endl;
}

void HttpApiConnector::send_event_information(const string& information, 
        const string& timestamp) {
    
    // Start a thread to do the dirty work
    std::thread th(send_to_server, this->new_host_for_api_server, 
            std::to_string(this->new_port_for_api_server), information, timestamp);
    th.detach();
}

void HttpApiConnector::detach_accept_callback(
        void (*callback) (std::vector<uint8_t>)) {

    // start server on thread
    auto server_func = [callback]() {
        while(true) {
            SerialDataParser parser;
            using SocketUtilities::SocketRAII;
            SocketRAII server_sock = SocketUtilities::create_server_socket("8070");
            SocketRAII client_sock = accept(server_sock);

            int n{-1};
            while(!parser && n) {
                char ch;
                n = SocketUtilities::recv(client_sock, &ch, 1);
                parser.process_byte(ch);
            }

            if (!parser) { throw std::runtime_error("Incomplete data received"); }
            else { callback(parser.get_data()); }
        }
    };
    std::thread th(server_func);
    th.detach();
}

void send_to_server(string address, string port, 
        string information, string timestamp) {

    using SocketRAII = SocketUtilities::SocketRAII;
    SocketRAII sock = SocketUtilities::create_client_socket(
            address.c_str(), port.c_str());

    string to_send = information + string{","} + timestamp; 
    int length = to_send.size();
    to_send = std::to_string(length) + string(1, '\0') + to_send;

    std::vector<char> data_to_send (to_send.begin(), to_send.end());
    send_all(sock, data_to_send);
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


    // receive the JSON response, 
    std::array<char, 1024> recv_buffer;
    int total_bytes_received {0};
    while(true) {
        auto bytes_received = SocketUtilities::recv(api_server_socket, 
                recv_buffer.data() + total_bytes_received, recv_buffer.size(), 0);

        // break if 0 bytes were received, throw error if -1 returned
        total_bytes_received += bytes_received;
        if (!bytes_received) {
            break;
        } else if (bytes_received == -1) {
            throw std::runtime_error("Could not establish handshake with server");
        }
    }
    cout << "DATA RECEIVED " << endl;
    cout.write(recv_buffer.data(), total_bytes_received);

    return vector<char> (recv_buffer.data(), 
            recv_buffer.data() + total_bytes_received);
}

map<string, string> parse_json_from_api_server_response(
        const vector<char>& response) {
    
    // initialize the stringstream with the response
    stringstream ss_used_to_parse_json;
    string string_response (response.begin(), response.end());
    ss_used_to_parse_json << string_response;

    // the set to be returned
    map<string, string> to_return;

    // take out the crap, i.e. read till the ':' in JSON and then read in the
    // integer.  This is because the JSON is of the form
    //     {"port":8080}
    int port_number_for_new_server;
    string host_for_new_server;

    // THIS IS NASTY! TODO PLEASE FUCKING CHANGE THIS
    getline(ss_used_to_parse_json, string_response, '{');
    getline(ss_used_to_parse_json, string_response, ':');
    if (string_response.find("port") != string::npos) {

        // if reading in the port number failed then throw a runtime error
        if (!ss_used_to_parse_json) {
            throw std::runtime_error("Could not parse the address for the "
                    "new API server");
        }

        getline(ss_used_to_parse_json, string_response, ',');
        stringstream temp_ss;
        temp_ss << string_response;
        temp_ss >> port_number_for_new_server;
        to_return["port"] = std::to_string(port_number_for_new_server);

        getline(ss_used_to_parse_json, string_response, ':');
        if (!ss_used_to_parse_json) {
            throw std::runtime_error("Could not parse the address for the "
                    "new API server");
        }
        ss_used_to_parse_json >> host_for_new_server;
        to_return["host"] = host_for_new_server;

    } else {

        // if reading in the port number failed then throw a runtime error
        if (!ss_used_to_parse_json) {
            throw std::runtime_error("Could not parse out the new port number for "
                    "the sub-API server");
        }

        getline(ss_used_to_parse_json, string_response, ',');
        stringstream temp_ss;
        temp_ss << string_response;
        temp_ss >> host_for_new_server;
        to_return["host"] = host_for_new_server;

        getline(ss_used_to_parse_json, string_response, ':');
        if (!ss_used_to_parse_json) {
            throw std::runtime_error("Could not parse the address for the "
                    "new API server");
        }
        ss_used_to_parse_json >> host_for_new_server;
        to_return["port"] = host_for_new_server;

    }

    // get reference and then remove
    string& host = to_return["host"];
    host.erase(std::remove_if(host.begin(), host.end(), 
                [](char ch) { return ch == '\"'; }), host.end());
    return to_return;
}
