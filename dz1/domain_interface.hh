#pragma once

#include "domain_element.hh"

class DomainInterface {
public:
	virtual       int              getCardinality() const = 0;
	virtual const DomainInterface* getComponent(int) const = 0;
	virtual       int              getNumberOfComponents() const = 0;
	virtual       int              indexOfElement(const DomainElement&) const = 0;
	virtual       DomainElement    elementForIndex(int) const = 0;

	virtual ~DomainInterface() {};
public:
	static constexpr int ELEMENT_NOT_PRESENT{-1};
};
