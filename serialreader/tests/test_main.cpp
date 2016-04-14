#include <cstdint>
#include <vector>
#include <iostream>
#include <string>
#include "HttpApiConnector.hpp"
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::vector;

void print_message(vector<uint8_t> message);

int main(int argc, char** argv) {

    if (argc != 3) {
        cerr << "Usage : " << argv[0] << " <api_server_hostname> <port>" << endl;
        return 1;
    }

    __attribute__((unused)) HttpApiConnector& connector = 
        HttpApiConnector::get_connector(argv[1], argv[2]);

    connector.detach_accept_callback(print_message);

    while(true);

    return 0;
}

void print_message(vector<uint8_t> message) {
    for (char ch : message) {
        cout << ch;
    } cout << endl;
}
