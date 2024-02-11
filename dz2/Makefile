CXX=g++
CXXFLAGS=-std=c++17 -Wall
LDFLAGS=
LDLIBS=

OBJECTS := $(patsubst %.cc,%.o,$(wildcard *.cc))

.PHONY: build
build: $(OBJECTS)
	$(CXX) -o program $(OBJECTS)

.PHONY: run
run: build
	./program.exe
