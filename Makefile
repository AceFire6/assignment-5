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
	rm -f $(OBJECTS) build/$(EXE_NAME) $(wildcard build/*.raw) build/unit_tests

run: default
	cd ./build && ./$(EXE_NAME) $(ARGS)

unit_tests.o:
	$(CC) -c src/unit_tests.cpp -o src/unit_tests.o $(FLAGS)

run-tests: unit_tests.o
	$(CC) src/unit_tests.o -o build/unit_tests $(FLAGS) && ./build/unit_tests
