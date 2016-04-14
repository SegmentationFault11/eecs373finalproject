#include "SerialDataParser.hpp"
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
using std::vector;
using std::string;
using std::cout;
using std::endl;

SerialDataParser::SerialDataParser() : state{State::READING_SIZE}, 
    message_length{-1}, current_position{0} {}

void SerialDataParser::process_byte(uint8_t byte) {
    
    switch (this->state) {
    case (State::READING_SIZE):
        
        if ( !(byte >= '0' && byte <= '9') && byte != 0 ) { // if not
            throw std::runtime_error("Expecting an integer, got " + 
                    std::to_string(byte));
        }

        // if null encountered then shift state and convert message_length to
        // integer
        if (!byte) {

            // set the message length for the message, recycle the data vector
            // and shift state
            this->message_length = stoi(string(this->data.begin(), 
                        this->data.end()));
            this->data.resize(0);
            
            // shift state
            this->state = State::READING_DATA;
            return;
        }

        this->data.push_back(byte);
        break;

    case (State::READING_DATA):
        
        // read in byte
        ++this->current_position;
        this->data.push_back(byte);

        // if read enough then set state to be done
        if (this->current_position >= this->message_length) { // sanity >=?
            this->state = State::DONE;
        }

        break;

    case (State::DONE):
        // noop
        break;
    }
}

const vector<uint8_t>& SerialDataParser::get_data() const {
    
    if (this->state != State::DONE) {
        throw std::runtime_error("Called get_data in an invalid state");
    }

    return this->data;
}

SerialDataParser::operator bool() const {
    return (this->state == State::DONE);
}
