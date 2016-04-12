#include <iostream>
#include <ftdi.h>
using namespace std;

int main() {

    struct ftdi_context ftdi;
    ftdi_init(&ftdi);

    cout << "Hello World" << endl;

    return 0;
}
