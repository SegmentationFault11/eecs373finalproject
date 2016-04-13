#include <iostream>
#include <stdexcept>
#include <chrono>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
using std::cout;
using std::cerr;
using std::endl;

int set_interface_attribs (int fd, int speed, int parity) {

    termios tty;
    memset (&tty, 0, sizeof(tty));
    if (tcgetattr (fd, &tty) != 0) {
        cerr << "error " << errno << "from tcgetattr" << endl;
        throw std::runtime_error("error from tcsetattr");
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
        cerr << "error " << errno << "from tcgetattr" << endl;
        throw std::runtime_error("error on tcgetattr");
    }

    return 0;
}

void set_blocking (int fd, int should_block)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0) {
        cerr << "error " << errno << "from tcgetattr" << endl;
        throw std::runtime_error("error from tcgetattr");
    }

    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    if (tcsetattr (fd, TCSANOW, &tty) != 0) {
        cerr << "error " << errno << "from tcsetattr" << endl;
        throw std::runtime_error("error from tcsetattr");
    }
}


int main() {
    const char *portname = "/dev/cu.usbserial-A603H8A5";
    int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        cerr << "error " << errno << "from tcgetattr" << endl;
        return 1;
    }

    // set BAUDrate as 9600, parity 0 and two stop bits
    set_interface_attribs(fd, B9600, 0); 
    // set non blocking
    set_blocking(fd, 0); 

    char receive_buffer [10000];
    while (true) {

        // write UART
        write(fd, "hello there!!\n", 14); 
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // read
        int received_till_now{0};
        int received{0};
        while ( (received = read(fd, receive_buffer + received_till_now,
                        sizeof(receive_buffer) - received_till_now)) ) {
            received_till_now += received;
        }

        // print received data
        if (received_till_now) {
            cout << "Received " << received_till_now << " bytes" << endl;
            cout.write(receive_buffer, received_till_now);
            cout << endl;
        }
    }

    return 0;
}
