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
using std::to_string;

/* used to lock cout */
mutex cout_mutex;

/* Forwards an HTTP message over to the USB device */
void forward_message_through_usb(vector<uint8_t> message);

/* Converts an event from the game to an informative string */
string convert_event_to_string(const vector<uint8_t>& event);

int main(int argc, char** argv) {

    if (argc != 3) {
        cerr << "Usage : " << argv[0] << " <api_server_hostname> <port>" << endl;
        return 1;
    }

    // get the connector and set the callback for all events
    HttpApiConnector& connector = 
        HttpApiConnector::get_connector(argv[1], argv[2]);
    connector.detach_set_callback(forward_message_through_usb);

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
        string event_information = convert_event_to_string(data);
        { unique_lock<mutex> lck {cout_mutex};
            cout << "event being sent is " << event_information << endl;
        }
        connector.send_event_information(event_information);
    }

    return 0;
}

void forward_message_through_usb(vector<uint8_t> message) {

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

string convert_event_to_string(const vector<uint8_t>& event) {
    
    AsyncSerialCommunicator& communicator = 
        AsyncSerialCommunicator::get_serial_reader();

    string to_return{""};

    // FAIL IF SOMETHING WENT WRONG
    assert(event.size() == 14);
    assert(!event[0]);
    assert(!event[13]);
    assert(event[2] == 'e');

    // Check what happened
    to_return += string{"Player "} + to_string(event[3]);
    to_return += (!event[4]) ? (string{" died! "}) : (string{" event! "});
    if (event[5] != std::numeric_limits<uint8_t>::max()) {
        to_return += (string{"Killed by player "} + to_string(event[5]));

        // send upgrade
        communicator.send_data(vector<uint8_t> {0, 
                static_cast<uint8_t>(event[5] + 1), 
                'u', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    }

    // update health
    if (event[4]) {
        to_return += string{"Health lowered to "} + to_string(event[4]);
    }
    
    return to_return;
}

