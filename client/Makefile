CPP=g++
CXXFLAGS=-Wextra -Wall -Wpedantic -O0 -g3 -std=c++17 #-L"ws2_32"
LDFLAGS=-lstdc++fs #-lwsock32 -lWs2_32

SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
HDRS=$(wildcard *.h)
BIN=client_app

all: $(BIN)

$(BIN): $(OBJS)
	$(CPP) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CPP) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf *.o client_app