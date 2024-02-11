#pragma once

#include "fuzzy_set_interface.hh"
#include "domain_interface.hh"
#include "int_unary_function.hh"

class CalculatedFuzzySet : public FuzzySetInterface {
public:
	CalculatedFuzzySet(DomainInterface* d, IntUnaryFunction* f);

	DomainInterface* getDomain() override;
	double           getValueAt(const DomainElement&) const override;
private:
	DomainInterface*  domain;
	IntUnaryFunction* function;
};
