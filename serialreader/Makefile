COMPILER = g++
THIS_DIR = .
SUBMODULES_DIR = ../submodules
LIBUSB_DIR = /usr/local/Cellar/libusb/1.0.20/include/libusb-1.0/
LIBFTDI_DIR = /usr/local/Cellar/libftdi/1.2/include/libftdi1/
FLAGS = -std=c++14 -I ${THIS_DIR} -I ${SUBMODULES_DIR} -I ${LIBUSB_DIR} -I ${LIBFTDI_DIR}

all: HttpApiConnector.cpp SerialDataParser.cpp
	$(COMPILER) $(FLAGS) cppsockets.a HttpApiConnector.cpp SerialDataParser.cpp tests/test_main.cpp

reader: communicate_through_uart.cpp
	$(COMPILER) $(FLAGS) -L /usr/local/Cellar/libftdi/1.2/lib/ -l ftdi1 communicate_through_uart.cpp -o reader

parser: tests/test_serial_data_parser.cpp SerialDataParser.cpp
	$(COMPILER) $(FLAGS) tests/test_serial_data_parser.cpp SerialDataParser.cpp

clean:
	rm -f a.out
	rm -f *.o
	rm -f reader
