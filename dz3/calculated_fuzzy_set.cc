#include "calculated_fuzzy_set.hh"

#include <stdexcept>

CalculatedFuzzySet::CalculatedFuzzySet(DomainInterface* d, IntUnaryFunction* f):
	domain{d}, function{f} {
	if (domain == nullptr) {
		throw std::invalid_argument("the domain must not be null");
	}
	if (domain->getNumberOfComponents() != 1) {
		throw std::invalid_argument("the domain must have only one component");
	}
	if (function == nullptr) {
		throw std::invalid_argument("the function must not be null");
	}
}

DomainInterface* CalculatedFuzzySet::getDomain() {
	return domain;
}

double CalculatedFuzzySet::getValueAt(const DomainElement& e) const {
	if (e.getNumberOfComponents() != 1) {
		throw std::invalid_argument("the element must have only one component");
	}
	return function->valueAt(e.getComponentValue(0));
}
