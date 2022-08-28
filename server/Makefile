CPP=g++

ifeq ($(OS),Windows_NT)
		CXXFLAGS=-Wextra -Wall -Wpedantic -O0 -g3 -std=c++17 -L"ws2_32"
		LDFLAGS=-lstdc++fs -lwsock32 -lWs2_32
else

	UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
		CXXFLAGS=-Wextra -Wall -Wpedantic -O0 -g3 -std=c++17
		LDFLAGS=-lstdc++fs
	endif
endif


SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
HDRS=$(wildcard *.h)
BIN=server_app

all: $(BIN)

$(BIN): $(OBJS)
	$(CPP) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CPP) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf *.o server_app