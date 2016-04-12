COMPILER = g++
WEBSERVER_DIR = ..
LIBUSB_DIR = /usr/local/Cellar/libusb/1.0.20/include/libusb-1.0/
LIBFTDI_DIR = /usr/local/Cellar/libftdi/1.2/include/libftdi1/
FLAGS = -std=c++14 -I ${WEBSERVER_DIR} -I ${LIBUSB_DIR} -I ${LIBFTDI_DIR}

all: HttpApiConnector.cpp 
	$(COMPILER) $(FLAGS) cppsockets.a HttpApiConnector.cpp test_main.cpp

reader: reader_test.cpp
	$(COMPILER) $(FLAGS) -L /usr/local/Cellar/libftdi/1.2/lib/ -l ftdi1 reader_test.cpp -o reader

clean:
	rm -f a.out
	rm -f *.o
	rm -f reader
