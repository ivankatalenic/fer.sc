#include "operations.hh"

#include "fuzzy_functions.hh"
#include "mutable_fuzzy_set.hh"

#include <stdexcept>

FuzzySetInterface* Operations::unaryOperation(FuzzySetInterface* s, FuzzyUnaryFunction* f) {
	if (s == nullptr) {
		throw std::invalid_argument("the fuzzy set can't be null");
	}
	if (f == nullptr) {
		throw std::invalid_argument("the function can't be null");
	}

	DomainInterface* d{s->getDomain()};

	MutableFuzzySet* res = new MutableFuzzySet(d);
	
	for (int i{0}; i < d->getCardinality(); ++i) {
		const DomainElement& e{d->elementForIndex(i)};
		const double val{s->getValueAt(e)};

		res->set(e, f->valueAt(val));
	}

	return res;
}

FuzzySetInterface* Operations::binaryOperation(FuzzySetInterface* s1, FuzzySetInterface* s2, FuzzyBinaryFunction* f) {
	if (s1 == nullptr || s2 == nullptr) {
		throw std::invalid_argument("the fuzzy set can't be null");
	}
	if (f == nullptr) {
		throw std::invalid_argument("the function can't be null");
	}

	DomainInterface* d1{s1->getDomain()};
	DomainInterface* d2{s2->getDomain()};

	if (d1->getCardinality() != d2->getCardinality()) {
		throw std::invalid_argument("the domains must be of equal cardinality");
	}
	if (d1->getNumberOfComponents() != d2->getNumberOfComponents()) {
		throw std::invalid_argument("the domains must have the same number of components");
	}

	MutableFuzzySet* res = new MutableFuzzySet(d1);
	
	for (int i{0}; i < d1->getCardinality(); ++i) {
		const DomainElement& e1{d1->elementForIndex(i)};
		
		const int index{d2->indexOfElement(e1)};
		if (index == DomainInterface::ELEMENT_NOT_PRESENT) {
			throw std::invalid_argument("the domains don't have the same elements");
		}
		const DomainElement& e2{d2->elementForIndex(index)};
		
		const double val1{s1->getValueAt(e1)};
		const double val2{s2->getValueAt(e2)};

		res->set(e1, f->valueAt(val1, val2));
	}

	return res;
}

FuzzyUnaryFunction* Operations::zadehNot() {
	return new ZadehNot();
}

FuzzyBinaryFunction* Operations::zadehAnd() {
	return new ZadehAnd();
}

FuzzyBinaryFunction* Operations::zadehOr() {
	return new ZadehOr();
}

FuzzyBinaryFunction* Operations::hamacherTNorm(double v) {
	return new HamacherTNorm(v);
}

FuzzyBinaryFunction* Operations::hamacherSNorm(double v) {
	return new HamacherSNorm(v);
}
