SHELL=/bin/bash

SRC = $(wildcard src/*.cpp)
INC = $(wildcard inc/*.hpp)
OBJ = $(SRC:src/%.cpp=obj/%.o)
BIN = bin/gentml

$(BIN): $(OBJ)
	g++-8 $(OBJ) -o $@ 

$(OBJ):obj/%.o:src/%.cpp $(INC)
	g++-8 -c -std=gnu++17 -Iinc $< -o $@

clean:
	rm -rf obj/*