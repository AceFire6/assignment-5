.SUFFIXES:
.SUFFIXES: .cpp .o

CC=g++
FLAGS=-std=c++11
SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXE_NAME=samp

default: $(OBJECTS)
	$(CC) $(SOURCES) -o build/$(EXE_NAME) $(FLAGS)

.cpp.o:
	$(CC) -c $< -o $@ $(FLAGS)

clean:
	rm -f $(OBJECTS) build/$(EXE_NAME)

run: default
	cd ./build && ./$(EXE_NAME) $(ARGS)
