COMPILER = g++
WEBSERVER_DIR = ..
FLAGS = -std=c++14 -O3 -Wall -Wvla -Werror -Wextra -pedantic -I ${WEBSERVER_DIR}

all: HttpApiConnector.cpp 
	$(COMPILER) $(FLAGS) cppsockets.a HttpApiConnector.cpp test_main.cpp

reader: reader_test.cpp
	$(COMPILER) $(FLAGS) -I /usr/local/Cellar/libftdi/1.2/include/libftdi1/ -L /usr/local/Cellar/libftdi/1.2/lib/ -l ftdi1 reader_test.cpp -o reader

clean:
	rm -f a.out
	rm -f *.o
	rm -f reader
