#include "SerialDataParser.hpp"
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
using std::cout;
using std::string;
using std::endl;

int main() {

    SerialDataParser parser;
    string data_stream ("5\0ari", 5);
    for (int i = 0; i < static_cast<int>(data_stream.size()) && !parser; ++i) {
        parser.process_byte(data_stream[i]);
    }
    string data {parser.get_data().begin(), parser.get_data().end()};
    assert(data == "ari");

    data_stream = string("3\0a", 3);
    parser = SerialDataParser{};
    for (int i = 0; i < static_cast<int>(data_stream.size()) && !parser; ++i) {
        parser.process_byte(data_stream[i]);
    }
    data = string{parser.get_data().begin(), parser.get_data().end()};
    assert(data == "a");

    data_stream = string("10\0abcdefg", 10);
    parser = SerialDataParser{};
    for (int i = 0; i < static_cast<int>(data_stream.size()) && !parser; ++i) {
        parser.process_byte(data_stream[i]);
    }
    data = string{parser.get_data().begin(), parser.get_data().end()};
    assert(data == "abcdefg");

    return 0;
}
