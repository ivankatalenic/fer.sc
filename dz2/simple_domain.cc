#include "simple_domain.hh"

#include "domain_element.hh"

#include <stdexcept>

SimpleDomain::SimpleDomain(int f, int l):
	first{f}, last{l} {
	if (first > last) {
		throw std::domain_error("the first bound must be less than the last bound");
	}
}

int SimpleDomain::getCardinality() const {
	return last - first;
}

const DomainInterface* SimpleDomain::getComponent(int index) const {
	if (index < 0) {
		throw std::out_of_range("index must be 0");
	}
	if (index >= 1) {
		throw std::out_of_range("index must be 0");
	}
	
	return this;
}

int SimpleDomain::getNumberOfComponents() const {
	return 1;
}

int SimpleDomain::indexOfElement(const DomainElement& e) const {
	if (e.getNumberOfComponents() != 1) {
		throw std::invalid_argument("simple domain contains only elements with one value");
	}

	const int val{e.getComponentValue(0)};
	if (val < first || val >= last) {
		return DomainInterface::ELEMENT_NOT_PRESENT;
	}

	return val - first;
}

DomainElement SimpleDomain::elementForIndex(int index) const {
	if (index < 0) {
		throw std::out_of_range("index must be greater than 0");
	}
	if (index >= getCardinality()) {
		throw std::out_of_range("index must be less than the domain's cardinality");
	}

	return DomainElement{first + index};
}

bool SimpleDomain::operator==(const SimpleDomain& other) const {
	if (first != other.first || last != other.last) {
		return false;
	}

	return true;
}

bool SimpleDomain::operator==(const DomainInterface& other) const {
	try {
		const SimpleDomain& o{dynamic_cast<const SimpleDomain&>(other)};

		return *this == o;
	} catch (const std::bad_cast& e) {
		return false;
	}
}

