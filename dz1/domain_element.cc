#include "domain_element.hh"

#include <string>
#include <initializer_list>
#include <stdexcept>
#include <iostream>

DomainElement::DomainElement(std::initializer_list<int> l): values{l} {}

DomainElement::DomainElement(const std::vector<int>& v): values{v} {}

int DomainElement::getNumberOfComponents() const {
	return values.size();
}

int DomainElement::getComponentValue(int index) const {
	if (static_cast<std::size_t>(index) >= values.size()) {
		throw std::out_of_range("index is greater than the dimension");
	}
	if (index < 0) {
		throw std::out_of_range("index is less than 0");
	}

	return values[index];
}

std::string DomainElement::toString() const {
	std::string result;

	result += '(';
	for (const auto& v : values) {
		result += std::to_string(v) + ", ";
	}
	result.erase(result.end() - 2, result.end());
	result += ')';

	return result;
}

bool operator==(const DomainElement& a, const DomainElement& b) {
	return a.values == b.values;
}

std::ostream& operator<<(std::ostream& out, const DomainElement& e) {
	out << e.toString();
	return out;
}
