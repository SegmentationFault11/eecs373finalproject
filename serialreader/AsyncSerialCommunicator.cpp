#include <functional>
#include <cstdlib>
#include <iterator>
#include <algorithm>
#include <string>
#include <array>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <cstdint>
#include "AsyncSerialCommunicator.hpp"
using std::cout;
using std::cerr;
using std::endl;
using std::unique_lock;
using std::mutex;
using std::vector;
using std::string;

/* Initialize statics */
static const char *portname = "/dev/cu.usbserial-A603H8A5";
static const string FILE_PATH_FOR_USB = "/dev/cu.usbserial-A603H8A5";
AsyncSerialCommunicator* AsyncSerialCommunicator::s_serial_data_reader = nullptr;
/* Mutexes and CVs used for synchronization */
static std::mutex mtx; /* for synchronizing private data */
static std::mutex send_receive_mtx; /* for synchronizing sends and receives */
static std::condition_variable cv;
static std::condition_variable cv_wait_for_all;
static const int DATA_PACKET_SIZE_FROM_HARDWARE = 14;

/* *********** static file scope functions ********** */
void accept_data(AsyncSerialCommunicator& communicator);

/* *********** function definitions *********** */
AsyncSerialCommunicator::AsyncSerialCommunicator() : has_detached{false}, 
    is_data_available{false} {}

AsyncSerialCommunicator& AsyncSerialCommunicator::get_serial_reader() {

    // acquire mutex, RAII
    { unique_lock<mutex> lck{mtx};

        // singleton pattern
        if (!AsyncSerialCommunicator::s_serial_data_reader) {
            AsyncSerialCommunicator::s_serial_data_reader = 
                new AsyncSerialCommunicator{};
        }
        return *AsyncSerialCommunicator::s_serial_data_reader;
    }
}

void AsyncSerialCommunicator::detach_and_start_accepting() {

    // acquire mutex, RAII
    { unique_lock<mutex> lck {mtx};

        // assert that another thread has not already called the detach
        // function
        assert(!this->has_detached);

        std::thread th(accept_data, std::ref(*this));
        th.detach();
        this->has_detached = true;
    }
}

vector<uint8_t> AsyncSerialCommunicator::get_data() {

    // block while data is not available.  Standard monitor pattern
    { unique_lock<mutex> lck {mtx};
        while (!this->is_data_available) {
            cv.wait(lck);
        }

        // make copy and return
        std::vector<uint8_t> copied_buffer (this->data_buffer);
        this->data_buffer.clear();
        is_data_available = false;
        return copied_buffer;
    }
}

void AsyncSerialCommunicator::send_data(const vector<uint8_t>& data) {

    // assert, detach thread and then send data in the thread
    assert(data.size() <= BUFFER_SIZE_MAX);
    assert(data.size() <= DATA_PACKET_SIZE_FROM_HARDWARE);

    auto detached_send_func = [data, this]() { // copy vec
        { unique_lock<mutex> lck{send_receive_mtx};

            // send data to the queue
            this->responses_to_send.push(std::array<uint8_t, BUFFER_SIZE_MAX>{});
            std::copy(data.begin(), data.end(), 
                    this->responses_to_send.front().data());
        }
    };
    std::thread detached_sending_thread(detached_send_func);
    detached_sending_thread.detach();
}

void AsyncSerialCommunicator::wait_for_all_sends_to_finish() {

    // simple wait on a condition_variable
    { unique_lock<mutex> lck {send_receive_mtx};
        while(!this->responses_to_send.empty()) {
            cv_wait_for_all.wait(lck);
        }
    }
}


/* *********** Implementation.  Here comes the nasty kernel stuff *********** */

/* Sets interface things, like the parity, baudrate, stop bits */
void set_interface_attributes(int fd, int speed, int parity);
/* Sets the file descriptor as blocking if should_block is 1 */
void set_blocking(int fd, int should_block);
/* Send all the data that has been accumulated to send_data calls */
void send_all_data(AsyncSerialCommunicator& communicator);
/* Adds the read in data to the back of the data vector in the communicator */
void accumulate_received_data_and_notify(AsyncSerialCommunicator& communicator, 
        uint8_t* begin, uint8_t* end);

/* THIS HAS TO START IN A DETACHED DAEMON THREAD */
void accept_data(AsyncSerialCommunicator& communicator) {

    communicator.fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (communicator.fd < 0) {
        throw std::runtime_error(string{"error "} + strerror(errno) + 
                string{"from tcgetattr"});
    }

    // set BAUDrate as 9600, parity 0 and two stop bits
    set_interface_attributes(communicator.fd, B9600, 0); 
    // set non blocking
    set_blocking(communicator.fd, 0); 

    std::array<uint8_t, AsyncSerialCommunicator::BUFFER_SIZE_MAX> receive_buffer;
    while (true) {

        // send all data that has accumulated, this waits for a bit for UART
        send_all_data(communicator);

        // read in a synchronized block
        int received_till_now{0};
        int received{0};
        { unique_lock<mutex> lck {send_receive_mtx};
            while ( (received = ::read(communicator.fd, 
                            receive_buffer.data() + received_till_now,
                            sizeof(receive_buffer) - received_till_now)) ) {
                received_till_now += received;
            }
        }

        // print received data
        if (received_till_now) {

            assert(received_till_now <= static_cast<int>(sizeof(receive_buffer)));

            // write data to the internal buffers for the communicator and
            // signal any threads that were waiting on the get_data() function
            accumulate_received_data_and_notify(communicator, 
                    receive_buffer.data(), 
                    receive_buffer.data() + received_till_now);

        } // if (received_till_now)
    } // while (true)
}

void accumulate_received_data_and_notify(AsyncSerialCommunicator& communicator, 
        uint8_t* begin, uint8_t* end) {

    // lock and append data to the back of the vector, and signal the
    // condition variable that data has been read
    { unique_lock<mutex> lck {mtx};

        // if distance is lower than what it should be append with data
        int distance_begin_end = std::distance(begin, end);
        if ((distance_begin_end % DATA_PACKET_SIZE_FROM_HARDWARE) != 0) {
            std::fill(begin + distance_begin_end, 
                    begin + distance_begin_end + 
                        (distance_begin_end % DATA_PACKET_SIZE_FROM_HARDWARE), 
                    0);
            std::advance(end, DATA_PACKET_SIZE_FROM_HARDWARE % distance_begin_end);
        }

        // move the end iterator and copy
        std::copy(begin, end, std::back_inserter(communicator.data_buffer));

        // signal the waiting threads
        communicator.is_data_available = true;
        cv.notify_one();
    }
}


void send_all_data(AsyncSerialCommunicator& communicator) {
    { unique_lock<mutex> lck {send_receive_mtx};

        // send all responses back
        while (!communicator.responses_to_send.empty()) {

            // write to the file descriptor, wait for UART to be happy :)
            ::write(communicator.fd, 
                    communicator.responses_to_send.front().data(), 
                    DATA_PACKET_SIZE_FROM_HARDWARE); 
            communicator.responses_to_send.pop();
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }

        // signal the main thread if it has called wait_for_all_sends_to_finish
        // cv precondition fixed before
        cv_wait_for_all.notify_one();
    }
}

void set_interface_attributes (int fd, int speed, int parity) {

    termios tty;
    memset (&tty, 0, sizeof(tty));
    if (tcgetattr (fd, &tty) != 0) {
        throw std::runtime_error(string{"error "} + std::to_string(errno) + 
                string{"from tcsetattr"});
    }

    // set the baud rate for the UART channel
    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    // set the width for characters
    tty.c_cflag |= CS8;     // 8-bit chars

    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing, TODO DONT KNOW 
    tty.c_lflag = 0;                // no signaling chars, no echo, TODO DONT KNOW

    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays, TODO DONT KNOW

    // read will not block, but if there is a character that was sent then the
    // read will wait for 0.5 seconds.  See 'man termios' and then search for
    // c_cc
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    // tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    // Set no parity, two stop bits and some random flow control of output
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag |= CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    // output error on error
    if (tcsetattr (fd, TCSANOW, &tty) != 0) {
        throw std::runtime_error(string{"error "} + strerror(errno) + 
                string{"from tcgetattr"});
    }
}

void set_blocking (int fd, int should_block) {
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0) {
        throw std::runtime_error(string{"error "} + strerror(errno) + 
                string{"from tcgetattr"});
    }

    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    if (tcsetattr (fd, TCSANOW, &tty) != 0) {
        throw std::runtime_error(string{"error "} + strerror(errno) + 
                string{"from tcgetattr"});
    }
}


