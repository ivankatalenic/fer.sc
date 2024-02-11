#include "domain_interface.hh"
#include "domain_builder.hh"
#include "domain_element.hh"

#include "fuzzy_set_interface.hh"
#include "mutable_fuzzy_set.hh"
#include "calculated_fuzzy_set.hh"
#include "function_builder.hh"

#include "operations.hh"

#include "debug.hh"

#include <iostream>
#include <string>

int main() {
	DomainInterface* d1{DomainBuilder::intRange(0, 11)};
	MutableFuzzySet m(d1);
	m.set({0}, 1.0);
	m.set({1}, 0.8);
	m.set({2}, 0.6);
	m.set({3}, 0.4);
	m.set({4}, 0.2);
	FuzzySetInterface* s1{&m};
	Debug::printFuzzy(s1, "s1:");

	FuzzySetInterface* not_s1{Operations::unaryOperation(s1, Operations::zadehNot())};
	Debug::printFuzzy(not_s1, "not_s1:");

	FuzzySetInterface* not_or_s1{Operations::binaryOperation(s1, not_s1, Operations::zadehOr())};
	Debug::printFuzzy(not_or_s1, "not_or_s1:");

	FuzzySetInterface* and_s1{Operations::binaryOperation(s1, not_or_s1, Operations::hamacherSNorm(0.7))};
	Debug::printFuzzy(and_s1, "and_s1:");

	return 0;
}
