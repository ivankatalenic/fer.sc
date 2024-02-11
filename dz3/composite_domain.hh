#pragma once

#include "domain_interface.hh"
#include "simple_domain.hh"

#include <initializer_list>
#include <vector>

class CompositeDomain : public DomainInterface {
public:
	CompositeDomain(std::initializer_list<DomainInterface*> l);

	int                    getCardinality() const override;
	const DomainInterface* getComponent(int) const override;
	int                    getNumberOfComponents() const override;
	int                    indexOfElement(const DomainElement& e) const override;
	DomainElement          elementForIndex(int) const override;

	bool operator==(const CompositeDomain& other) const;
	bool operator==(const DomainInterface& other) const override;
private:
	std::vector<DomainInterface*> values;
};
