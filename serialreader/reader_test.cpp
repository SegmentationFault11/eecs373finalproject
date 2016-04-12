#include <fstream>
#include <iostream>
#include <cassert>
#include <libusb.h>
#include <ftdi.h>
using namespace std;

int main() {

    struct ftdi_context ftdi;
    ftdi_init(&ftdi);

    ifstream fin("/dev/cu.usbserial-A603H8A5");
    if (!fin) {
        cerr << "Problem" << endl;
        return 1;
    }

    char ch;
    while (1) {
        fin.get(ch);
        cout << ch;
    }

    cout << "Hello World" << endl;

    return 0;
}
