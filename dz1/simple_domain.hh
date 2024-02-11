#pragma once

#include "domain_interface.hh"
#include "domain_element.hh"

class SimpleDomain : public DomainInterface {
public:
	SimpleDomain(int first, int last);

	int                    getCardinality() const override;
	const DomainInterface* getComponent(int index) const override;
	int                    getNumberOfComponents() const override;
	int                    indexOfElement(const DomainElement& e) const override;
	DomainElement          elementForIndex(int index) const override;
private:
	int first;
	int last;
};
