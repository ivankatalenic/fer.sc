#pragma once

#include <string>
#include <vector>
#include <initializer_list>
#include <iostream>

class DomainElement {
public:
	DomainElement(std::initializer_list<int> l);
	DomainElement(const std::vector<int>& v);

	int getNumberOfComponents() const;
	int getComponentValue(int index) const;

	std::string toString() const;

	friend bool operator==(const DomainElement& a, const DomainElement& b);
private:
	std::vector<int> values;
};

std::ostream& operator<<(std::ostream& out, const DomainElement& e);
