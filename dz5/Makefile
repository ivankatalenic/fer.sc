CXX=g++
CXXFLAGS=-std=c++17 -Wall -Iinclude
LDFLAGS=
LDLIBS=-lwxbase31u -lwxmsw31u_core
LDPATHS=-Llib/gcc810_x64_dll
LINKSFLAGS=

OBJECTS := $(patsubst %.cc,%.o,$(wildcard *.cc))

.PHONY: build
build: $(OBJECTS)
	$(CXX) -o program $(OBJECTS) $(LINKFLAGS) $(LDPATHS) $(LDLIBS)

.PHONY: run
run: build
	./program.exe
