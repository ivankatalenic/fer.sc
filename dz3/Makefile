CXX=g++
CXXFLAGS=-Wall
LDFLAGS=
LDLIBS=
LDPATHS=
LINKSFLAGS=

MAINS := main.o single.o multi.o
OBJECTS := $(patsubst %.cc,%.o,$(wildcard *.cc))
DEPS := $(filter-out $(MAINS),$(OBJECTS))


.PHONY: build
build: $(OBJECTS)
	$(MAKE) build-main
	$(MAKE) build-single
	$(MAKE) build-multi

.PHONY: build-main
build-main: main.o $(DEPS)
	$(CXX) -o program main.o $(DEPS) $(LINKFLAGS) $(LDPATHS) $(LDLIBS)

.PHONY: build-single
build-single: single.o $(DEPS)
	$(CXX) -o single single.o $(DEPS) $(LINKFLAGS) $(LDPATHS) $(LDLIBS)

.PHONY: build-multi
build-multi: multi.o $(DEPS)
	$(CXX) -o multi multi.o $(DEPS) $(LINKFLAGS) $(LDPATHS) $(LDLIBS)

.PHONY: run
run: build-main
	java -jar Simulator.jar

.PHONY: clean
clean:
	rm -f $(MAINS) $(DEPS)