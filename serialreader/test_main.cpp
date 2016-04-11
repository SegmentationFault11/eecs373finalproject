#include <iostream>
#include <string>
#include "HttpApiConnector.hpp"
using std::cout;
using std::endl;
using std::cerr;
using std::string;

int main(int argc, char** argv) {

    if (argc != 3) {
        cerr << "Usage : " << argv[0] << " <api_server_hostname> <port>" << endl;
        return 1;
    }

    __attribute__((unused)) HttpApiConnector& connector = 
        HttpApiConnector::get_connector(argv[1], argv[2]);

    return 0;
}
