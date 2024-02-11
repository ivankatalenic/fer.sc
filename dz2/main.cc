#include "domain_interface.hh"
#include "domain_builder.hh"
#include "domain_element.hh"

#include "simple_domain.hh"
#include "composite_domain.hh"

#include "fuzzy_set_interface.hh"
#include "mutable_fuzzy_set.hh"
#include "calculated_fuzzy_set.hh"
#include "function_builder.hh"

#include "operations.hh"
#include "relations.hh"

#include "debug.hh"

#include <iostream>
#include <string>

int main2() {
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

int main3() {
	SimpleDomain sd1(0, 5);
	SimpleDomain sd2(0, 5);
	SimpleDomain sd3(2, 7);

	CompositeDomain cd1{&sd1, &sd2};
	CompositeDomain cd2{&sd1, &sd2};
	CompositeDomain cd3{&sd1, &sd3};

	DomainInterface* di1{&sd1};
	DomainInterface* di2{&sd2};
	DomainInterface* di3{&cd1};
	DomainInterface* di4{&cd1};
	DomainInterface* di5{&cd3};

	std::cout << (sd1 == sd2) << std::endl;
	std::cout << (sd1 == sd3) << std::endl;
	std::cout << (sd1 == cd1) << std::endl << std::endl;

	std::cout << (cd1 == cd2) << std::endl;
	std::cout << (cd1 == cd3) << std::endl << std::endl;
	
	std::cout << (*di1 == *di2) << std::endl;
	std::cout << (*di1 == *di3) << std::endl << std::endl;

	std::cout << (*di3 == *di4) << std::endl;
	std::cout << (*di3 == *di5) << std::endl << std::endl;

	return 0;
}

int main4() {
	DomainInterface* u{DomainBuilder::intRange(1, 6)};
	DomainInterface* domain{DomainBuilder::combine(u, u)};

	MutableFuzzySet mf1(domain);
	mf1.set({1, 1}, 1);
	mf1.set({2, 2}, 1);
	mf1.set({3, 3}, 1);
	mf1.set({4, 4}, 1);
	mf1.set({5, 5}, 1);
	mf1.set({3, 1}, 0.5);
	mf1.set({1, 3}, 0.5);
	FuzzySetInterface* r1{&mf1};

	MutableFuzzySet mf2(domain);
	mf2.set({1, 1}, 1);
	mf2.set({2, 2}, 1);
	mf2.set({3, 3}, 1);
	mf2.set({4, 4}, 1);
	mf2.set({5, 5}, 1);
	mf2.set({3, 1}, 0.5);
	mf2.set({1, 3}, 0.1);
	FuzzySetInterface* r2{&mf2};

	MutableFuzzySet mf3(domain);
	mf3.set({1, 1}, 1);
	mf3.set({2, 2}, 1);
	mf3.set({3, 3}, 0.3);
	mf3.set({4, 4}, 1);
	mf3.set({5, 5}, 1);
	mf3.set({1, 2}, 0.6);
	mf3.set({2, 1}, 0.6);
	mf3.set({2, 3}, 0.7);
	mf3.set({3, 2}, 0.7);
	mf3.set({3, 1}, 0.5);
	mf3.set({1, 3}, 0.5);
	FuzzySetInterface* r3{&mf3};

	MutableFuzzySet mf4(domain);
	mf4.set({1, 1}, 1);
	mf4.set({2, 2}, 1);
	mf4.set({3, 3}, 1);
	mf4.set({4, 4}, 1);
	mf4.set({5, 5}, 1);
	mf4.set({1, 2}, 0.4);
	mf4.set({2, 1}, 0.4);
	mf4.set({2, 3}, 0.5);
	mf4.set({3, 2}, 0.5);
	mf4.set({3, 1}, 0.4);
	mf4.set({1, 3}, 0.4);
	FuzzySetInterface* r4{&mf4};

	std::cout << "r1 is U*U: "       << Relations::isUxU(r1)       << std::endl;
	std::cout << "r1 is reflexive: " << Relations::isReflexive(r1) << std::endl;
	std::cout << "r1 is symmetric: " << Relations::isSymmetric(r1) << std::endl;
	
	std::cout << "r2 is symmetric: " << Relations::isSymmetric(r2) << std::endl;

	std::cout << "r3 is reflexive: "          << Relations::isReflexive(r3)        << std::endl;
	std::cout << "r3 is max-min-transitive: " << Relations::isMaxMinTransitive(r3) << std::endl;
	
	std::cout << "r4 is max-min-transitive: " << Relations::isMaxMinTransitive(r4) << std::endl;

	return 0;
}

int main5() {
	DomainInterface* u1{DomainBuilder::intRange(1, 5)};
	DomainInterface* u2{DomainBuilder::intRange(1, 4)};
	DomainInterface* u3{DomainBuilder::intRange(1, 5)};

	MutableFuzzySet mf1(DomainBuilder::combine(u1, u2));
	mf1.set({1, 1}, 0.3);
	mf1.set({1, 2}, 1);
	mf1.set({3, 3}, 0.5);
	mf1.set({4, 3}, 0.5);
	FuzzySetInterface *r1{&mf1};

	MutableFuzzySet mf2(DomainBuilder::combine(u2, u3));
	mf2.set({1, 1}, 1);
	mf2.set({2, 1}, 0.5);
	mf2.set({2, 2}, 0.7);
	mf2.set({3, 3}, 1);
	mf2.set({3, 4}, 0.4);
	FuzzySetInterface *r2{&mf2};

	FuzzySetInterface* r1r2{Relations::compositionOfBinaryRelations(r1, r2)};

	DomainInterface* d{r1r2->getDomain()};
	for (int i{0}; i < d->getCardinality(); ++i) {
		const DomainElement& e{d->elementForIndex(i)};

		std::cout << "mi(" << e << ") = " << r1r2->getValueAt(e) << std::endl;
	}

	return 0;
}

int main() {
	DomainInterface* u{DomainBuilder::intRange(1, 5)};

	MutableFuzzySet mf(DomainBuilder::combine(u, u));
	mf.set({1, 1}, 1);
	mf.set({2, 2}, 1);
	mf.set({3, 3}, 1);
	mf.set({4, 4}, 1);
	mf.set({1, 2}, 0.3);
	mf.set({2, 1}, 0.3);
	mf.set({2, 3}, 0.5);
	mf.set({3, 2}, 0.5);
	mf.set({3, 4}, 0.2);
	mf.set({4, 3}, 0.2);
	FuzzySetInterface* r{&mf};

	std::cout << "Starting relation is fuzzy equivalence: " << Relations::isFuzzyEquivalence(r) << '\n' << std::endl;

	FuzzySetInterface* c{r};
	for (int i{1}; i <= 3; ++i) {
		c = Relations::compositionOfBinaryRelations(c, r);

		std::cout << "Composition #" << i << std::endl;
		
		std::cout << "Relation:" << std::endl;
		DomainInterface* d{c->getDomain()};
		for (int i{0}; i < d->getCardinality(); ++i) {
			const DomainElement& e{d->elementForIndex(i)};

			std::cout << "mi(" << e << ") = " << c->getValueAt(e) << std::endl;
		}

		std::cout << "Is fuzzy equivalence now: " << Relations::isFuzzyEquivalence(c) << "\n\n" << std::endl;
	}

	return 0;
}
