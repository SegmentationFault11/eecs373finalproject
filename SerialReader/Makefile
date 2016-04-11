COMPILER = g++
WEBSERVER_DIR = ..
FLAGS = -std=c++14 -O3 -Wall -Wvla -Werror -Wextra -pedantic -I ${WEBSERVER_DIR}

all: HttpApiConnector.cpp 
	$(COMPILER) $(FLAGS) cppsockets.a HttpApiConnector.cpp test_main.cpp

clean:
	rm -f a.out
	rm -f *.o
