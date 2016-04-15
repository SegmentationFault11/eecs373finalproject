#include <cstdint>
#include <cassert>
#include <vector>
#include <iostream>
#include <string>
#include <mutex>
#include "HttpApiConnector.hpp"
#include "AsyncSerialCommunicator.hpp"
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::vector;
using std::mutex;
using std::unique_lock;

mutex cout_mutex;

void print_message(vector<uint8_t> message);

int main(int argc, char** argv) {

    if (argc != 3) {
        cerr << "Usage : " << argv[0] << " <api_server_hostname> <port>" << endl;
        return 1;
    }

    // get the connector and set the callback for all events
    HttpApiConnector& connector = 
        HttpApiConnector::get_connector(argv[1], argv[2]);
    connector.detach_set_callback(print_message);

    // get the serial communicator
    AsyncSerialCommunicator& communicator = 
        AsyncSerialCommunicator::get_serial_reader();
    communicator.detach_and_start_accepting();

    while(true) {
        auto data = communicator.get_data();
        { unique_lock<mutex> lck{cout_mutex};
            cout << "Data received from SmartFusion : [";
            for (int ch : data) {
                    cout << ch << ' ';
            } cout << endl;
        }
    }

    return 0;
}

void print_message(vector<uint8_t> message) {

    assert(message.size() == 14);
    assert(!message[0] && !message[13]);

    AsyncSerialCommunicator& communicator = 
        AsyncSerialCommunicator::get_serial_reader();

    { unique_lock<mutex> lck{cout_mutex};
        cout << "Got message from web app : [";
        for (int ch : message) {
            cout << ch << ' ';
        } cout << "]" << endl;
    }

    // forward the message though the serial device to the other side
    communicator.send_data(message);
}
