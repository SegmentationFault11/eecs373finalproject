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

#include <vector>
#include <cstdint>

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

private:
    AsyncSerialCommunicator();
    static AsyncSerialCommunicator* s_serial_data_reader;

    /* private data members */
    bool has_detached = false;
    bool is_data_available = false;
    vector<uint8_t> data_buffer;
    /* Mutexes and CVs used for synchronization */
    std::mutex mtx; /* for synchronizing private data */
    std::mutex send_receive_mtx; /* for synchronizing sends and receives */
    std::condition_variable cv;
    /* the usb file descriptor */
    int fd;
};
