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

        vector<vector<uint8_t>> data_gotten_from_device;
        while (data.size() >= 14) {
            vector<uint8_t> current_data (data.begin(), data.begin() + 14);
            for(int i = 0; i < 14; ++i) {
                data.erase(data.begin());
            }
            data_gotten_from_device.push_back(current_data);
        }

        while (!data_gotten_from_device.empty()) {
            string event_information = convert_event_to_string(
                    data_gotten_from_device.front());
            data_gotten_from_device.erase(data_gotten_from_device.begin());
            if (event_information.empty()) { 
                cout << "IGNORING EVENT" << endl;
                continue; 
            }
            { unique_lock<mutex> lck {cout_mutex};
                cout << "Size of data_gotten_from_device is " 
                    << data_gotten_from_device.size() << endl;
                cout << "event being sent is " << event_information << endl;
            }
            connector.send_event_information(event_information);
        }
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
    // assert(event.size() == 14);
    // assert(!event[0]);
    // assert(!event[13]);
    // assert(event[2] == 'e');
    // if (event.size() != 14 || event[0] || event[13] || event[2] != 'e') {
    //     return string{""};
    // }
    bool does_have_e = false;
    int e_index = -1;
    for (int i = 0; i < 14; ++i) {
        if (event[i] == 'e') {
            e_index = i;
            does_have_e = true;
        }
    }
    if (!does_have_e) { return string{""}; }

    // Check what happened
    to_return += string{"Player "} + to_string(event[e_index + 1]);
    to_return += (!event[e_index + 2]) ? (string{" died! "}) : (string{" event! "});
    if (event[e_index + 3] != std::numeric_limits<uint8_t>::max() && 
            !event[e_index + 2]) {
        to_return += (string{"Killed by player "} + to_string(event[e_index + 3]) + 
                string{"! "});

        // send upgrade
        communicator.send_data(vector<uint8_t> {0, 
                static_cast<uint8_t>(event[e_index + 3] + 1), 
                'u', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    }

    // update health
    if (event[e_index + 2]) {
        to_return += string{"Health lowered to "} + to_string(event[e_index + 2]);
    }
    
    return to_return;
}

