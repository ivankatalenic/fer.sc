#pragma once

#include "fuzzy_set_interface.hh"
#include "domain_interface.hh"
#include "domain_element.hh"

#include <vector>

class MutableFuzzySet : public FuzzySetInterface {
public:
	MutableFuzzySet(DomainInterface* d);

	DomainInterface* getDomain() override;
	double           getValueAt(const DomainElement&) const override;

	MutableFuzzySet& set(const DomainElement& e, double val);
private:
	DomainInterface*    domain;
	std::vector<double> memberships;
};
