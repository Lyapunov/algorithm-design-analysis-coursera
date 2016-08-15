# Magic:
# g++ -std=c++11  miniapp.cpp -o miniapp

CC = g++
CFLAGS =-Wall -std=c++11

CPP_FILES := $(wildcard src/*.cpp)
APP_FILES := $(addprefix app/,$(notdir $(CPP_FILES:.cpp=)))
PWD := $(shell pwd)

all: app $(APP_FILES)

app:
	mkdir -p app

app/%: src/%.cpp
	$(CC) $(CFLAGS) -O3 -o $@ $<

clean:
	rm -rf app


# Creating input files from zipped inputs:
# ----------------------------------------
GZ_FILES := $(wildcard zipped_inputs/*.gz)

inputs: inputsdir $(GZ_FILES)

inputsdir:
	mkdir -p inputs

zipped_inputs/%: inputsdir
	@src/input_uncompress $@ $(PWD)/inputs/

# Solving:
# --------
solve: solutions.txt

solutions.txt:
	@echo Creating solutions.txt
	@./run_all.sh >solutions.txt 2>&1 
