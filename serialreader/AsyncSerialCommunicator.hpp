/*
 * Written by Aaryaman Sagar
 * AsyncSerialCommunicator.hpp
 *
 * This module contains the machine specific code used to read through the UART
 * protocol.  It is a singleton since it should only be used at one time.  So
 * instantiate the singleton with the call to the factory method.  
 *
 * The start receiving function then detaches from the main thread and starts
 * reading from the serial port on the computer.  
 *
 * The get_data() call blocks on a condition variable until there is data
 * available to be read
 */

#include <queue>
#include <array>
#include <vector>
#include <cstdint>
#include <mutex>
#include <condition_variable>

class AsyncSerialCommunicator {
public:

    /* Singleton pattern */
    static AsyncSerialCommunicator& get_serial_reader();

    /* 
     * Detach and start accepting data on the serial port.  Throws an
     * exception on exceptional conditions
     */
    void detach_and_start_accepting();

    /* Blocking call to wait for data to be available */
    std::vector<uint8_t> get_data();

    /* Non blocking call used to send data through UART to the device */
    void send_data(const std::vector<uint8_t>& data);

    /* Waits for all sent data to get finished sending */
    void wait_for_all_sends_to_finish();

private:
    AsyncSerialCommunicator();
    static AsyncSerialCommunicator* s_serial_data_reader;

    /* private data members */
    bool has_detached = false;
    bool is_data_available = false;
    std::vector<uint8_t> data_buffer;
    static const int BUFFER_SIZE_MAX = 1024;
    std::queue<std::array<uint8_t, 1024>> responses_to_send;
    /* the usb file descriptor */
    int fd;

    /* friends */
    friend void set_interface_attributes(int fd, int speed, int parity);
    friend void set_blocking(int fd, int should_block); 
    friend void accept_data(AsyncSerialCommunicator& communicator);
    friend void send_all_data(AsyncSerialCommunicator& communicator);
    friend void accumulate_received_data_and_notify(
            AsyncSerialCommunicator& communicator,
            uint8_t* begin, uint8_t* end); 
};

