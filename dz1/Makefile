SOURCES = \
	main.cc \
	domain_element.cc \
	simple_domain.cc \
	composite_domain.cc \
	domain_builder.cc \
	calculated_fuzzy_set.cc \
	mutable_fuzzy_set.cc \
	function_builder.cc \
	lamma_function.cc \
	lambda_function.cc \
	operations.cc

.PHONY: build run

build:
	g++ -std=c++17 -Wall -o program $(SOURCES)

run: build
	./program.exe
