#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <vector>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include "AsyncSerialCommunicator.hpp"
#include "HttpApiConnector.hpp"
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

void print_message(vector<uint8_t> message);

int main() {


    // set up the connection to server
    if (argc != 3) {
        cerr << "Usage : " << argv[0] << " <api_server_hostname> <port>" << endl;
        return 1;
    }
    __attribute__((unused)) HttpApiConnector& connector = 
        HttpApiConnector::get_connector(argv[1], argv[2]);
    connector.detach_accept_callback(print_message);

    AsyncSerialCommunicator& communicator = 
        AsyncSerialCommunicator::get_serial_reader();
    communicator.detach_and_start_accepting();

    for (int i = 0; i < 10; ++i) {
        auto data = communicator.get_data();
        cout << "data received : ";
        for (auto ch : data) {
            cout << ch;
        } cout << endl;
        
        // send a reply for each incoming message, which is of length 14
        for (size_t i = 0; i < data.size() / 14; ++i) {
            string response = "indeedyourerig";
            vector<uint8_t> response_vec (response.begin(), response.end());
            communicator.send_data(response_vec);
        }

    }

    // wait for all sends to finish
    communicator.wait_for_all_sends_to_finish();

    return 0;
}

void print_message(vector<uint8_t> message) {
    AsyncSerialCommunicator& communicator = 
        AsyncSerialCommunicator::get_serial_reader();
}
