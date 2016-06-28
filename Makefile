# Magic:
# g++ -std=c++11  miniapp.cpp -o miniapp

CC = g++
CFLAGS =-Wall -std=c++11

CPP_FILES := $(wildcard src/*.cpp)
APP_FILES := $(addprefix app/,$(notdir $(CPP_FILES:.cpp=)))

all: $(APP_FILES)

app/%: src/%.cpp
	$(CC) $(CFLAGS) -o $@ $<
