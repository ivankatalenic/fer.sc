#include "mutable_fuzzy_set.hh"

#include <stdexcept>

MutableFuzzySet::MutableFuzzySet(DomainInterface* d):
	domain{d} {
	if (domain == nullptr) {
		throw std::invalid_argument("the domain must not be null");
	}

	const int card{domain->getCardinality()};
	memberships = std::vector(card, 0.0);
}

DomainInterface* MutableFuzzySet::getDomain() {
	return domain;
}

double MutableFuzzySet::getValueAt(const DomainElement& e) const {
	const int index{domain->indexOfElement(e)};
	if (index == DomainInterface::ELEMENT_NOT_PRESENT) {
		throw std::domain_error("the element must be inside of the set's core domain");
	}

	return memberships[index];
}

MutableFuzzySet& MutableFuzzySet::set(const DomainElement& e, double val) {
	const int index{domain->indexOfElement(e)};
	if (index == DomainInterface::ELEMENT_NOT_PRESENT) {
		throw std::domain_error("the element must be inside of the set's core domain");
	}

	memberships[index] = val;
	return *this;
}
