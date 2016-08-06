# Magic:
# g++ -std=c++11  miniapp.cpp -o miniapp

CC = g++
CFLAGS =-Wall -std=c++11

CPP_FILES := $(wildcard src/*.cpp)
APP_FILES := $(addprefix app/,$(notdir $(CPP_FILES:.cpp=)))

all: app $(APP_FILES)

app:
	mkdir -p app

app/%: src/%.cpp
	$(CC) $(CFLAGS) -O3 -o $@ $<

clean:
	rm -rf app
