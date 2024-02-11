#pragma once

#include "domain_interface.hh"
#include "domain_element.hh"

class FuzzySetInterface {
public:
	virtual DomainInterface* getDomain() = 0;
	virtual double           getValueAt(const DomainElement&) const = 0;

	virtual ~FuzzySetInterface() {};
};
