/*
 * Written by Aaryaman Sagar
 * SerialDataParser.hpp
 *
 * This can be used in situations where data is sent through byte streams of
 * variable length chunks.  For example this can be used for network
 * communication where data is sent through sockets and the receiver has no
 * clue about the number of bytes that are going to be received with one call
 * of the recv() function.  This module however requires that data be sent in
 * the following format.
 *
 *  <data_size><NULL><data>
 *
 * Where data_size is the length of the message including the number of bytes
 * used to encode the data_size parameter and the null character.  This parser
 * assumes the data will be encoded using a byte for one decimal digit.
 */

#include <cstdint>
#include <vector>

class SerialDataParser {
public:

    /* Default constructor */
    SerialDataParser();
    SerialDataParser(const SerialDataParser& other) = default;

    /* 
     * Adds a single byte to the state for the data parser.  Throws a
     * std::runtime_error when there an exceptional condition in the data
     * stream.
     */
    void process_byte(uint8_t byte);

    /* 
     * Returns the data as an array of bytes.  If this is called when the
     * parser is not in the DONE state, an exception will be thrown 
     */ 
    const std::vector<uint8_t>& get_data() const;

    /* 
     * Can be used to check whether the SerialDataParser object is in an
     * acceptable state.  Returns false when it is still reading.  Exceptional
     * conditions are flagged by exceptions.
     */
    operator bool() const;

private:

    /* An enum type used to keep track of the current state for the object */
    enum class State { READING_SIZE, READING_DATA, DONE };
    State state;

    /* An integer that stores the length of the entire message */
    int message_length;
    /* An integer denoting the byte number that the parser is at */
    int current_position;
    /* An array used to store the data that is being parsed */
    std::vector<uint8_t> data;
};
